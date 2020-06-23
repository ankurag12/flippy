#ifndef LSM6DS33_H_
#define LSM6DS33_H_

#include <cstdint>
#include <map>

typedef std::uint8_t byte;

enum Axis { X = 0, Y = 2, Z = 4 };

class LSM6DS33 {
public:
  LSM6DS33(int host_id = -1);

  bool init(uint odr_accel = 1660, uint fs_accel = 2,
            uint filter_bw_accel = 200, uint odr_gyro = 1660,
            uint fs_gyro = 245);

  double get_accel_reading(Axis axis);
  double get_gyro_reading(Axis axis);

private:
  uint _odr_accel;
  uint _fs_accel;
  uint _filter_bw_accel;
  uint _odr_gyro;
  uint _fs_gyro;

  int _host_id = -1;
  int _i2c_handle;

  static constexpr byte _device_address = 0x6b;
  static constexpr byte _who_am_i_id = 0x69;
  static constexpr byte _dout_word_length = 16;
  static constexpr uint _dout_word_fs = (1 << _dout_word_length) - 1;

  // Map for full scale for the accelerometer in +/- g
  static const std::map<uint, byte> _fs_accel_map;

  // Map for anti-aliasing filter bandwidth for the accelerometer in Hz
  static const std::map<uint, byte> _filter_bw_accel_map;

  // Map for full scale for the gyro in +/- deg/s
  static const std::map<uint, byte> _fs_gyro_map;

  // Map for output data rate for accelerometer (ODR_XL) in Hz
  static const std::map<uint, byte> _odr_accel_map;

  // Map for output data rate for the gyro (ODR_G) in Hz
  static const std::map<uint, byte> _odr_gyro_map;

  // Not all registers are listed here
  enum _RegisterAddress {
    // Embedded functions configuration register
    FUNC_CFG_ACCESS = 0x01,

    ORIENT_CFG_G = 0x0B,

    // Who I am ID
    WHO_AM_I = 0x0F,

    // Accelerometer and gyroscope control registers
    CTRL1_XL = 0x10,
    CTRL2_G = 0x11,
    CTRL3_C = 0x12,
    CTRL4_C = 0x13,
    CTRL5_C = 0x14,
    CTRL6_C = 0x15,
    CTRL7_G = 0x16,
    CTRL8_XL = 0x17,
    CTRL9_XL = 0x18,
    CTRL10_C = 0x19,
    STATUS_REG = 0x1E,

    // Gyroscope output registers
    OUTX_L_G = 0x22,
    OUTX_H_G = 0x23,
    OUTY_L_G = 0x24,
    OUTY_H_G = 0x25,
    OUTZ_L_G = 0x26,
    OUTZ_H_G = 0x27,

    // Accelerometer output registers
    OUTX_L_XL = 0x28,
    OUTX_H_XL = 0x29,
    OUTY_L_XL = 0x2A,
    OUTY_H_XL = 0x2B,
    OUTZ_L_XL = 0x2C,
    OUTZ_H_XL = 0x2D

  };
};

#endif // LSM6DS33_H_
