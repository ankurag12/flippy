#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "LSM6DS33.h"
#include "Motor.h"

class Controller {
public:
  Controller();
  bool flip();
  bool balance(double distance_setpoint);
  bool set_linear_speed(double speed_mps);
  bool set_angular_speed(double speed_dps);
  // ballpark for default values:
  // max linear speed at no load (430 rpm) and 60mm dia wheel = 1.35 m/s, at max
  // power (220 rpm) = 0.7 m/s
  bool move_linear_dist(double speed_mps = 0.5, double distance_m = 1);
  // ballpark for default values:
  // max angular speed at no load (430 rpm), 60mm dia wheel and 10cm wheelbase =
  // 27 deg/s, at max power (220 rpm) = 13.8 deg/s
  bool move_angular_dist(double speed_dps = 10, double angle_deg = 90);
  bool stop();
  double get_tilt_angle();

private:
  static constexpr double _wheel_dia_m = 0.06;
  static constexpr double _wheelbase_m = 0.1;
  static constexpr double _motor_no_load_rpm = Motor::no_load_rpm;
  static constexpr double _motor_gear_ratio = Motor::gear_ratio;
  static constexpr uint _motor_enc_counts_per_rev = Motor::enc_counts_per_rev;
  static constexpr uint _imu_odr_accel = 1660;  // Hz
  static constexpr uint _imu_fs_accel = 2;      // g
  static constexpr uint _imu_filter_bw_accel = 200; // Hz
  static constexpr uint _imu_odr_gyro = 1660;   // Hz
  static constexpr uint _imu_fs_gyro = 500;     // deg/s
  double _max_linear_speed;
  double _max_rotational_speed;
  int _host_id;
  Motor _left_motor;
  Motor _right_motor;
  LSM6DS33 _imu;

  double _get_gyro_bias();
};

#endif // CONTROLLER_H_