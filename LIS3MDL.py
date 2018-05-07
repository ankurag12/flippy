from enum import Enum, IntEnum
import time
import smbus2
import logging

class LIS3MDL:
    _DEVICE_ADDRESS = 0x1E
    _WHO_AM_I_ID = 0b00111101
    _DOUT_WORD_LENGTH = 16    # in number of bits
    _TEMP_FS = 125    # -40 deg C to 85 deg C

    # Dictionary for Output Data Rate in Hz (when FAST_ODR disabled)
    _ODR_DICT = {0.625: 0b000,
                 1.25: 0b001,
                 2.5: 0b010,
                 5: 0b011,
                 10: 0b100,
                 20: 0b101,
                 40: 0b110,
                 80: 0b111}
    
    # Dictionary for Output Data Rate in Hz (when FAST_ODR enabled)
    _FAST_ODR_DICT = {1000: 0b00,
                      560: 0b01,
                      300: 0b10,
                      155: 0b11}

    # Dictionary for Full Scale for the magnetometer in +/- gauss
    _FS_DICT = {4: 0b00,
                8: 0b01,
                12: 0b10,
                16: 0b11}
    
    # Not all registers are listed here
    class _RegAddr(IntEnum):
        
        # Who I am
        WHO_AM_I = 0x0F
        
        # Control registers
        CTRL_REG1 = 0x20
        CTRL_REG2 = 0x21
        CTRL_REG3 = 0x22
        CTRL_REG4 = 0x23
        CTRL_REG5 = 0x24

        # Status register
        STATUS_REG = 0x27

        # Magnetometer outpur registers
        OUT_X_L = 0x28
        OUT_X_H = 0x29
        OUT_Y_L = 0x2A
        OUT_Y_H = 0x2B
        OUT_Z_L = 0x2C
        OUT_Z_H = 0x2D
        
        # Temperature output
        TEMP_OUT_L = 0x2E
        TEMP_OUT_H = 0x2F
    

    def __init__(self, odr, fs, temp_sens):
        self._odr = odr
        self._fs = fs
        self._temp_sens = temp_sens
        
        self._bus = smbus2.SMBus(1)
        id = self._bus.read_byte_data(self._DEVICE_ADDRESS, self._RegAddr.WHO_AM_I)
        assert(id == self._WHO_AM_I_ID)
        logging.info("Connection to LIS3MDL established successfully")
        
        # Power on and configure device
        TEMP_EN = int(self._temp_sens)
        if (self._odr <= 80):
            FAST_ODR = 0
            OM = 0b00
            OMZ = 0b00
            DO = self._ODR_DICT[self._odr]
            MD = 0b00
        else:
            FAST_ODR = 1
            OM = self._FAST_ODR_DICT[self._odr]
            OMZ = self._FAST_ODR_DICT[self._odr]
            DO = 0b00
            MD = 0b00

        FS = self._FS_DICT[self._fs]

        control1 = TEMP_EN<<7 | OM<<5 | DO<<2 | FAST_ODR<<1
        control2 = FS<<5
        control3 = MD<<1
        control4 = OMZ<<2
        
        self._bus.write_byte_data(self._DEVICE_ADDRESS, self._RegAddr.CTRL_REG1, control1)
        self._bus.write_byte_data(self._DEVICE_ADDRESS, self._RegAddr.CTRL_REG2, control2)
        self._bus.write_byte_data(self._DEVICE_ADDRESS, self._RegAddr.CTRL_REG3, control3)
        self._bus.write_byte_data(self._DEVICE_ADDRESS, self._RegAddr.CTRL_REG4, control4)

   
    def mag_reading(self, axis):
        if axis not in ['x', 'y', 'z']:
            raise ValueError("axis should be 'x', 'y' or 'z'")
        axis_offset =  (ord(axis) - ord('x')) * 2
        raw_bytes = self._bus.read_i2c_block_data(self._DEVICE_ADDRESS, self._RegAddr.OUT_X_L + axis_offset, 2)
        magfield = int.from_bytes(raw_bytes, byteorder='little', signed=True) / (2**self._DOUT_WORD_LENGTH / (2*self._fs))
        return magfield

    
    def temp_reading(self):
        if not self._temp_sens:
            raise ValueError("Must set temp_sens to True while instantiating")
        raw_bytes = self._bus.read_i2c_block_data(self._DEVICE_ADDRESS, self._RegAddr.TEMP_OUT_L, 2)
        temperature = int.from_bytes(raw_bytes, byteorder='little', signed=True) / 256.0 + 25.0
        return temperature


if __name__ == "__main__":
    try:
        magneto = LIS3MDL(odr=80, fs=4, temp_sens=True)
    except AssertionError as err:
        logging.error("Device ID does not match")
        raise err
    
    while True:
        print("magnetic field = ", magneto.mag_reading('x'))
        print("temp = ", magneto.temp_reading())
        time.sleep(0.10)
        
