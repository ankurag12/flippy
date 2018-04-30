import smbus2
import time
import sys

# Open i2c bus 1
bus = smbus2.SMBus(1)

b = bus.read_byte_data(0x1e, 0x0f)

print(b)

# Set accelerometer operating mode
data = 0b10000000
bus.write_byte_data(0x6b, 0x10, data)

while True :
    z_accel = bus.read_i2c_block_data(0x6b, 0x2c, 2)

    print(int.from_bytes(z_accel, byteorder='little', signed=True)/16384.0)
    time.sleep(0.1)
bus.close()
