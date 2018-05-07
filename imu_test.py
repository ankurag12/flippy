import smbus2
import time
import sys
from LSM6D import LSM6DS33

try:
    imu = LSM6DS33(odr_accel=1660, fs_accel=2, bw_accel=200, odr_gyro=1660, fs_gyro=245)
except AssertionError as err:
    logging.error("Device ID does not match")
    raise err

while True:
    print("z-accel = ", imu.accel_reading('z'))
    print("z-gyro = ", imu.gyro_reading('z'))
    time.sleep(0.10)
