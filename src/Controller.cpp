#include "Controller.h"
#include "ConfigReader.h"
#include "HwImpl.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#define M_PI 3.14159265358979323846 /* pi */

using namespace std::chrono_literals;

Controller::Controller()
    : _host_id(init_gpio()), _left_motor(Motor(MotorSide::LEFT, _host_id)),
      _right_motor(Motor(MotorSide::RIGHT, _host_id)),
      _imu(LSM6DS33(_host_id)) {
  // Initialize IMU
  _imu.init(_imu_odr_accel, _imu_fs_accel, _imu_filter_bw_accel, _imu_odr_gyro,
            _imu_fs_gyro);
}

bool Controller::move_linear_dist(double speed_mps, double distance_m) {
  double init_avg_ticks = (_left_motor.get_current_tick_count() +
                           _right_motor.get_current_tick_count()) /
                          2;
  // Read config file
  ConfigReader config_reader;
  PidConfig gain = config_reader.get_pid_gains("linear_dist_pid");

  double err = distance_m;
  double prev_err = 0;
  double integral_err = err;
  int low_error_run_count = 0;
  const int low_error_run_count_thresh = 3;
  const double err_change_thresh =
      0.0001; // In the units of the quantity being controlled i.e, meters
  double pwm_input;

  // Since we have seen cases where `err` isn't updated (spurious wake-ups from
  // sleep?), we look for a few consecutive runs with low change in error
  // instead of just one
  while (low_error_run_count < low_error_run_count_thresh) {
    pwm_input = gain.p * err + gain.i * integral_err;
    _left_motor.run(pwm_input);
    _right_motor.run(pwm_input);

    std::this_thread::sleep_for(10ms); // Loop rate (approximate)

    double delta_avg_ticks = (_left_motor.get_current_tick_count() +
                              _right_motor.get_current_tick_count()) /
                                 2 -
                             init_avg_ticks;
    double distance_travelled = delta_avg_ticks * (M_PI * _wheel_dia_m) /
                                (_motor_enc_counts_per_rev * _motor_gear_ratio);
    prev_err = err;
    err = distance_m - distance_travelled;
    integral_err += err;
    if (fabs(err - prev_err) < err_change_thresh) {
      low_error_run_count++;
    } else {
      low_error_run_count = 0;
    }
  }
  std::cout << "err = " << err << " PWM = " << pwm_input << std::endl;
  stop();
  return true;
}

bool Controller::stop() {
  _left_motor.run(0);
  _right_motor.run(0);
  return true;
}

bool Controller::flip() {
  _left_motor.run(1.0);
  _right_motor.run(1.0);
  std::this_thread::sleep_for(400ms);
  _left_motor.run(-1.0);
  _right_motor.run(-1.0);
  std::this_thread::sleep_for(400ms);
  stop();
  return true;
}

bool Controller::balance() {

  // Start with stop
  stop();

  // Read config file
  ConfigReader config_reader;
  PidConfig gain_tilt_angle =
      config_reader.get_pid_gains("balance_tilt_angle_pid");
  FilterConfig filter_weight =
      config_reader.get_filter_weights("tilt_calc_filter_weight");

  // Maximum angle over which it won't try to balance else motors would go crazy
  const double tilt_angle_max = 30.0;
  // Because of robot's unsymmetrical weight distribution
  const double tilt_angle_setpoint = -5.0;

  double err_tilt_angle = 0.0;
  double int_err_tilt_angle = 0.0;
  double prev_err_tilt_angle;
  double diff_err_tilt_angle;

  double pwm_input;

  double gyro_bias = _get_gyro_bias();
  double tilt_angle_filtered = 0.0;
  double tilt_angle_gyro;
  double tilt_angle_accel;

  auto tik = std::chrono::high_resolution_clock::now();

  // Run only if the robot angle is "recoverable"
  while (fabs(tilt_angle_filtered) < tilt_angle_max) {
    auto ang_rate = _imu.get_gyro_reading(Axis::Y) - gyro_bias;

    // std::this_thread::sleep_for(10ms);   // Optional
    tilt_angle_accel = atan2(_imu.get_accel_reading(Axis::Z),
                             _imu.get_accel_reading(Axis::X)) *
                       180 / M_PI;

    auto tok = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dt = tok - tik;
    tik = tok;
    tilt_angle_gyro = tilt_angle_filtered + ang_rate * dt.count();

    tilt_angle_filtered = filter_weight.alpha * tilt_angle_gyro +
                          (1 - filter_weight.alpha) * tilt_angle_accel;

    err_tilt_angle = tilt_angle_setpoint - tilt_angle_filtered;
    int_err_tilt_angle += err_tilt_angle;
    diff_err_tilt_angle = err_tilt_angle - prev_err_tilt_angle;

    pwm_input = gain_tilt_angle.p * err_tilt_angle +
                gain_tilt_angle.i * int_err_tilt_angle +
                gain_tilt_angle.d * diff_err_tilt_angle;
    
    _left_motor.run(-pwm_input);
    _right_motor.run(-pwm_input);
    prev_err_tilt_angle = err_tilt_angle;

    std::cout << "err = " << err_tilt_angle << " PWM = " << pwm_input
              << std::endl;
  }
  stop();
  std::cout << "Unrecoverable angle :( make robot stand upright manually and "
               "run again!"
            << std::endl;
  return false;
}

double Controller::get_tilt_angle() {
  // Read config file
  ConfigReader config_reader;
  FilterConfig filter_weight =
      config_reader.get_filter_weights("tilt_calc_filter_weight");

  double gyro_bias = _get_gyro_bias();
  double tilt_angle_filtered = 0.0;
  double tilt_angle_gyro = 0.0;
  double tilt_angle_accel;
  auto tik = std::chrono::high_resolution_clock::now();

  while (true) {
    auto ang_rate = _imu.get_gyro_reading(Axis::Y) - gyro_bias;

    // std::this_thread::sleep_for(10ms);
    tilt_angle_accel = atan2(_imu.get_accel_reading(Axis::Z),
                             _imu.get_accel_reading(Axis::X)) *
                       180 / M_PI;
    auto tok = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dt = tok - tik;
    tik = tok;
    tilt_angle_gyro = tilt_angle_filtered + ang_rate * dt.count();

    tilt_angle_filtered = filter_weight.alpha * tilt_angle_gyro +
                          (1 - filter_weight.alpha) * tilt_angle_accel;
    std::cout << tilt_angle_filtered << std::endl;
  }
}

double Controller::_get_gyro_bias() {
  // Estimate bias in gyro
  std::cout << "Calculating gyro bias, do not move it!" << std::endl;
  const int num_init_gyro_readings = 200;
  const int sleep_duration_us = (1000000 / _imu_odr_gyro) + 1;
  double total = 0.0;
  for (auto i = 0; i < num_init_gyro_readings; ++i) {
    total += _imu.get_gyro_reading(Axis::Y);
    std::this_thread::sleep_for(std::chrono::microseconds(sleep_duration_us));
  }
  std::cout << "Gyro bias calculation done." << std::endl;
  return total / num_init_gyro_readings;
}