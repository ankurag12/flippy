#include "Controller.h"
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
  _imu.init(1660, 2, 200, 1660, 245);
}

bool Controller::move_linear_dist(double speed_mps, double distance_m) {
  double init_avg_ticks = (_left_motor.get_current_tick_count() +
                           _right_motor.get_current_tick_count()) /
                          2;
  double p_gain = 5;
  double i_gain = 0.0;
  double d_gain = 0.0;

  double err = distance_m;
  double prev_err = 0;
  int low_error_run_count = 0;
  const int low_error_run_count_thresh = 3;
  const double err_change_thresh =
      0.0001; // In the units of the quantity being controlled i.e, meters

  // Since we have seen cases where `err` isn't updated (spurious wake-ups from
  // sleep?), we look for a few consecutive runs with low change in error
  // instead of just one
  while (low_error_run_count < low_error_run_count_thresh) {
    double pwm_input = p_gain * err;
    std::cout << " PWM = " << pwm_input << std::endl;
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
