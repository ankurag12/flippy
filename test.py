import smbus2
import time
import sys
from LSM6DS33 import LSM6DS33
from LIS3MDL import LIS3MDL

try:
    imu = LSM6DS33(odr_accel=1660, fs_accel=2, bw_accel=200, odr_gyro=1660, fs_gyro=245)
    magneto = LIS3MDL(odr=80, fs=4, temp_sens=True)

except AssertionError as err:
    logging.error("Device ID does not match")
    raise err

while True:
    print("z-accel = ", imu.accel_reading('z'))
    print("z-gyro = ", imu.gyro_reading('z'))
    print("magnetic field = ", magneto.mag_reading('x'))
    print("temp = ", magneto.temp_reading())    
    time.sleep(0.10)

        
