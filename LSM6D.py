from enum import Enum, IntEnum
import time
import smbus2
import logging

DEVICE_ADDRESS = 0x6B
WHO_AM_I_ID = 0x69
DOUT_WORD_LENGTH = 16    # in number of bits

FS_XL_DICT = {2: 0b00, 16: 0b01, 4: 0b10, 8: 0b11}           # in +/- g terms
BW_XL_DICT = {400: 0b00, 200: 0b01, 100: 0b10, 50: 0b11}   # in Hz
FS_G_DICT = {125: 0b001, 245: 0b000, 500: 0b010, 1000: 0b100, 2000: 0b110}    # in dps

ODR_XL = 0b1000
ODR_G = 0b1000

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
    def __init__(self, fs_accel, bw_accel, fs_gyro):   # TODO: implement ODR
        self._fs_accel = fs_accel
        self._bw_accel = bw_accel
        self._fs_gyro = fs_gyro
        
        self._bus = smbus2.SMBus(1)
        id = self._bus.read_byte_data(DEVICE_ADDRESS, RegAddr.WHO_AM_I)
        assert(id == WHO_AM_I_ID)
        logging.info("Connection to LSM6DS33 established successfully")
        
        # Power on and configure device
        FS_XL = FS_XL_DICT[self._fs_accel]
        BW_XL = BW_XL_DICT[self._bw_accel]
        mode_accel = ODR_XL<<4 | FS_XL<<2 | BW_XL
        self._bus.write_byte_data(DEVICE_ADDRESS, RegAddr.CTRL1_XL, mode_accel)
        
        FS_G = FS_G_DICT[self._fs_gyro]
        mode_gyro = ODR_G<<4 | FS_G<<2
        self._bus.write_byte_data(DEVICE_ADDRESS, RegAddr.CTRL2_G, mode_gyro)


    def accel_reading(self, axis):
        if axis not in ['x', 'y', 'z']:
            raise ValueError("axis should be 'x', 'y' or 'z'")
        axis_offset =  (ord(axis) - ord('x')) * 2
        raw_bytes = self._bus.read_i2c_block_data(DEVICE_ADDRESS, RegAddr.OUTX_L_XL + axis_offset, 2)
        accel = int.from_bytes(raw_bytes, byteorder='little', signed=True) / (2**DOUT_WORD_LENGTH / (2*self._fs_accel))
        return accel
    
    def gyro_reading(self, axis):
        if axis not in ['x', 'y', 'z']:
            raise ValueError("axis should be 'x', 'y' or 'z'")
        axis_offset =  (ord(axis) - ord('x')) * 2
        raw_bytes = self._bus.read_i2c_block_data(DEVICE_ADDRESS, RegAddr.OUTX_L_G + axis_offset, 2)
        ang_rate = int.from_bytes(raw_bytes, byteorder='little', signed=True) / (2**DOUT_WORD_LENGTH / (2*self._fs_gyro))
        return ang_rate


if __name__ == "__main__":
    try:
        imu = LSM6DS33(fs_accel=2, bw_accel=200, fs_gyro=245)
    except AssertionError as err:
        logging.error("Device ID does not match")
        raise err
    
    while True:
        print("z-accel = ", imu.accel_reading('z'))
        print("z_gyro = ", imu.gyro_reading('z'))
        time.sleep(0.10)
        

