from enum import Enum, IntEnum
import time
import smbus2
import logging

DEVICE_ADDRESS = 0x6b
ACCEL_FULL_SCALES = {0b00: 2, 0b01: 16, 0b10: 4, 0b11: 8}           # in +/- g terms
GYRO_FULL_SCALES = {0b00: 245, 0b01: 500, 0b10: 1000, 0b11: 2000}    # in dps
DOUT_WORD_LENGTH = 16    # in number of bits
WHO_AM_I_ID = 0x69

ODR_XL = 0b1000
FS_XL = 0b00
BW_XL = 0b00

ODR_G = 0b1000
FS_G = 0b00
FS_125 = 0

# Not all registers are listed here
class RegAddr(IntEnum):
    # Embedded functions configuration register
    FUNC_CFG_ACCESS = 0x01
    
    ORIENT_CFG_G = 0x0B
    
    # Who I am ID
    WHO_AM_I = 0x0F
    
    # Accelerometer and gyroscope control registers
    CTRL1_XL = 0x10
    CTRL2_G = 0x11
    CTRL3_C = 0x12
    CTRL4_C = 0x13
    CTRL5_C = 0x14
    CTRL6_C = 0x15
    CTRL7_G = 0x16
    CTRL8_XL = 0x17
    CTRL9_XL = 0x18
    CTRL10_C = 0x19
    STATUS_REG = 0x1E
    
    # Gyroscope output registers
    OUTX_L_G = 0x22
    OUTX_H_G = 0x23
    OUTY_L_G = 0x24
    OUTY_H_G = 0x25
    OUTZ_L_G = 0x26
    OUTZ_H_G = 0x27
    
    # Accelerometer output registers
    OUTX_L_XL = 0x28
    OUTX_H_XL = 0x29
    OUTY_L_XL = 0x2A
    OUTY_H_XL = 0x2B
    OUTZ_L_XL = 0x2C
    OUTZ_H_XL = 0x2D


class LSM6DS33:
    def __init__(self):
        self.bus = smbus2.SMBus(1)
        id = self.bus.read_byte_data(DEVICE_ADDRESS, RegAddr.WHO_AM_I)
        assert(id == WHO_AM_I_ID)
        logging.info("Connection to LSM6DS33 established successfully")
        
        # Power on and configure device
        accel_mode = ODR_XL<<4 | FS_XL<<2 | BW_XL
        self.bus.write_byte_data(DEVICE_ADDRESS, RegAddr.CTRL1_XL, accel_mode)
        
        gyro_mode = ODR_G<<4 | FS_G<<2 | FS_125<<1
        self.bus.write_byte_data(DEVICE_ADDRESS, RegAddr.CTRL2_G, gyro_mode)


    def accel_reading(self, axis):
        if axis not in ['x', 'y', 'z']:
            raise ValueError("axis should be 'x', 'y' or 'z'")
        axis_offset =  (ord(axis) - ord('x')) * 2
        raw_bytes = self.bus.read_i2c_block_data(DEVICE_ADDRESS, RegAddr.OUTX_L_XL + axis_offset, 2)
        accel = int.from_bytes(raw_bytes, byteorder='little', signed=True) / (2**DOUT_WORD_LENGTH / (2*ACCEL_FULL_SCALES[FS_XL]))
        return accel
    
    def gyro_reading(self, axis):
        if axis not in ['x', 'y', 'z']:
            raise ValueError("axis should be 'x', 'y' or 'z'")
        axis_offset =  (ord(axis) - ord('x')) * 2
        raw_bytes = self.bus.read_i2c_block_data(DEVICE_ADDRESS, RegAddr.OUTX_L_G + axis_offset, 2)
        if FS_125 == 1:
            gyro_full_scale = 125
        else:
            gyro_full_scale = GYRO_FULL_SCALES[FS_G]
        ang_rate = int.from_bytes(raw_bytes, byteorder='little', signed=True) / (2**DOUT_WORD_LENGTH / (2*gyro_full_scale))
        return ang_rate


if __name__ == "__main__":
    try:
        imu = LSM6DS33()
    except AssertionError as err:
        logging.error("Device ID does not match")
        raise err
    
    while True:
        print("z-accel = ", imu.accel_reading('z'))
        print("z_gyro = ", imu.gyro_reading('z'))
        time.sleep(0.10)
        

