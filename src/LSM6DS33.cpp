#include "LSM6DS33.h"

#include <errno.h>
#include <wiringPiI2C.h>

#include <cassert>
#include <cstdint>
#include <iostream>

const std::map<uint, byte> LSM6DS33::_fs_accel_map = {
    {2, 0b00}, {16, 0b01}, {4, 0b10}, {8, 0b11}};
const std::map<uint, byte> LSM6DS33::_filter_bw_accel_map = {
    {400, 0b00}, {200, 0b01}, {100, 0b10}, {50, 0b11}};
const std::map<uint, byte> LSM6DS33::_fs_gyro_map = {
    {125, 0b001}, {245, 0b000}, {500, 0b010}, {1000, 0b100}, {2000, 0b110}};
const std::map<uint, byte> LSM6DS33::_odr_accel_map = {
    {0, 0b0000},    {13, 0b0001},   {26, 0b0010},  {52, 0b0011},
    {104, 0b0100},  {208, 0b0101},  {416, 0b0110}, {833, 0b0111},
    {1660, 0b1000}, {3330, 0b1001}, {6660, 0b1010}};
const std::map<uint, byte> LSM6DS33::_odr_gyro_map = {
    {0, 0b0000},   {13, 0b0001},  {26, 0b0010},  {52, 0b0011},  {104, 0b0100},
    {208, 0b0101}, {416, 0b0110}, {833, 0b0111}, {1660, 0b1000}};

bool LSM6DS33::init(uint odr_accel, uint fs_accel, uint filter_bw_accel,
                    uint odr_gyro, uint fs_gyro) {
  // Setting values here instead of the constructor so that they can be re-set.
  _odr_accel = odr_accel;
  _fs_accel = fs_accel;
  _filter_bw_accel = filter_bw_accel;
  _odr_gyro = odr_gyro;
  _fs_gyro = fs_gyro;

  int ret;
  _dev_handle = wiringPiI2CSetup(_device_address);
  std::cout << "I2C init result: " << _dev_handle << std::endl;

  assert(wiringPiI2CReadReg8(_dev_handle, _RegisterAddress::WHO_AM_I) ==
         (int)_who_am_i_id);
  std::cout << "Device LSM6DS33 detected!" << std::endl;

  // Configure device
  byte odr_accel_ctrl = _odr_accel_map.at(_odr_accel);
  byte fs_accel_ctrl = _fs_accel_map.at(_fs_accel);
  byte filter_bw_accel_ctrl = _filter_bw_accel_map.at(_filter_bw_accel);
  byte accel_mode =
      odr_accel_ctrl << 4 | fs_accel_ctrl << 2 | filter_bw_accel_ctrl;
  ret =
      wiringPiI2CWriteReg8(_dev_handle, _RegisterAddress::CTRL1_XL, accel_mode);
  // if(ret < 0) return false;

  byte odr_gyro_ctrl = _odr_gyro_map.at(_odr_gyro);
  byte fs_gyro_ctrl = _fs_gyro_map.at(_fs_gyro);
  byte gyro_mode = odr_gyro_ctrl << 4 | fs_gyro_ctrl << 2;
  ret = wiringPiI2CWriteReg8(_dev_handle, _RegisterAddress::CTRL2_G, gyro_mode);

  // if(ret < 0) return false;
  return true;
}

double LSM6DS33::get_accel_reading(Axis axis) {
  byte raw_byte_lo = (byte)wiringPiI2CReadReg8(
      _dev_handle, _RegisterAddress::OUTX_L_XL + axis);
  byte raw_byte_hi = (byte)wiringPiI2CReadReg8(
      _dev_handle, _RegisterAddress::OUTX_H_XL + axis);
  int16_t accel_unscaled = raw_byte_hi << 8 | raw_byte_lo;
  return accel_unscaled / ((1 << _dout_word_length) / (2.0 * _fs_accel));
}

double LSM6DS33::get_gyro_reading(Axis axis) {
  byte raw_byte_lo =
      (byte)wiringPiI2CReadReg8(_dev_handle, _RegisterAddress::OUTX_L_G + axis);
  byte raw_byte_hi =
      (byte)wiringPiI2CReadReg8(_dev_handle, _RegisterAddress::OUTX_H_G + axis);
  int16_t gyro_unscaled = raw_byte_hi << 8 | raw_byte_lo;
  return gyro_unscaled / ((1 << _dout_word_length) / (2.0 * _fs_gyro));
}
