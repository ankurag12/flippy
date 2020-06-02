#include "Controller.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#define M_PI 3.14159265358979323846 /* pi */

using namespace std::chrono_literals;

Controller::Controller()
    : _left_motor(Motor(MotorSide::LEFT)),
      _right_motor(Motor(MotorSide::RIGHT)), _imu(LSM6DS33()) {

  // Initialize IMU
  _imu.init(1660, 2, 200, 1660, 245);
}

bool Controller::move_linear_dist(double speed_mps, double distance_m) {
  double current_avg_ticks = (_left_motor.get_current_tick_count() +
                              _right_motor.get_current_tick_count()) /
                             2;
  double required_avg_ticks = distance_m * _motor_enc_counts_per_rev *
                              _motor_gear_ratio / (M_PI * _wheel_dia_m);
  double target_avg_ticks = current_avg_ticks + required_avg_ticks;

  double p_gain = 0.001;
  double i_gain = 0.0;
  double d_gain = 0.0;

  double err = target_avg_ticks - current_avg_ticks;
  double prev_err = 0;
  int low_error_run_count = 0;
  const int low_error_run_count_thresh = 3;
  const double err_change_thresh = 0.01;

  // Since we have seen cases where `err` isn't updated (spurious wake-ups from
  // sleep?), we look for a few consecutive runs with low change in error
  // instead of just one
  while (low_error_run_count < low_error_run_count_thresh) {
    double pwm_input = p_gain * err;
    std::cout << " PWM = " << pwm_input << std::endl;
    _left_motor.run(pwm_input);
    _right_motor.run(pwm_input);

    std::this_thread::sleep_for(10ms); // Loop rate (approximate)

    current_avg_ticks = (_left_motor.get_current_tick_count() +
                         _right_motor.get_current_tick_count()) /
                        2;
    prev_err = err;
    err = target_avg_ticks - current_avg_ticks;
    std::cout << "err = " << err << " prev_err = " << prev_err << std::endl;
    if (fabs(err - prev_err) < err_change_thresh) {
      low_error_run_count++;
    } else {
      low_error_run_count = 0;
    }
  }

  stop();
  return true;
}

bool Controller::stop() {
  _left_motor.run(0);
  _right_motor.run(0);
  return true;
}
