#include "LSM6DS33.h"
#include "Motor.h"

class Controller {
public:
  Controller();
  bool flip();
  void balance();
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

private:
  static constexpr double _wheel_dia_m = 0.06;
  static constexpr double _wheelbase_m = 0.1;
  static constexpr double _motor_no_load_rpm = Motor::no_load_rpm;
  static constexpr double _motor_gear_ratio = Motor::gear_ratio;
  static constexpr uint _motor_enc_counts_per_rev = Motor::enc_counts_per_rev;
  double _max_linear_speed;
  double _max_rotational_speed;
  int _host_id;
  Motor _left_motor;
  Motor _right_motor;
  LSM6DS33 _imu;
};
