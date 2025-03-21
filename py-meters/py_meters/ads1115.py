from time import sleep

ADS1115_ADDRESS_1 = 0x48
ADS1115_ADDRESS_2 = 0x49
ADS1115_ADDRESS_3 = 0x4a
ADS1115_ADDRESS_4 = 0x4b

# Register and other configuration values:
ADS1115_DEFAULT_ADDRESS = 0x48
ADS1115_POINTER_CONVERSION = 0x00
ADS1115_POINTER_CONFIG = 0x01
ADS1115_POINTER_LOW_THRESHOLD = 0x02
ADS1115_POINTER_HIGH_THRESHOLD = 0x03
ADS1115_CONFIG_OS_SINGLE = 0x8000
ADS1115_CONFIG_MUX_OFFSET = 12

# Mapping of gain values to config register values.
ADS1115_CONFIG_GAIN = {
    2/3: 0x0000,
    1:   0x0200,
    2:   0x0400,
    4:   0x0600,
    8:   0x0800,
    16:  0x0A00
}
ADS1115_CONFIG_MODE_CONTINUOUS = 0x0000
ADS1115_CONFIG_MODE_SINGLE = 0x0100

# Mapping of data/sample rate to config register values for ADS1115 (slower).
ADS1115_CONFIG_DR = {
    8:    0x0000,
    16:   0x0020,
    32:   0x0040,
    64:   0x0060,
    128:  0x0080,
    250:  0x00A0,
    475:  0x00C0,
    860:  0x00E0
}
ADS1115_CONFIG_COMP_WINDOW = 0x0010
ADS1115_CONFIG_COMP_ACTIVE_HIGH = 0x0008
ADS1115_CONFIG_COMP_LATCHING = 0x0004
ADS1115_CONFIG_COMP_QUE = {
    1: 0x0000,
    2: 0x0001,
    4: 0x0002
}
ADS1115_CONFIG_COMP_QUE_DISABLE = 0x0003


class ADS1115(object):
    """Base functionality for ADS1x15 analog to digital converters."""

    def __init__(self, address=ADS1115_DEFAULT_ADDRESS, i2c=None, **kwargs):
        if i2c is None:
            import Adafruit_GPIO.I2C as I2C
            i2c = I2C
        self._device = i2c.get_i2c_device(address, busnum=1, **kwargs)

    def _data_rate_default(self):
        """Default from datasheet page 16, config register DR bit default."""
        return 128

    def _data_rate_config(self, data_rate):
        if data_rate not in ADS1115_CONFIG_DR:
            raise ValueError('Data rate must be one of: 8, 16, 32, 64, 128, 250, 475, 860')
        return ADS1115_CONFIG_DR[data_rate]

    def _conversion_value(self, low, high):
        # Convert to 16-bit signed value.
        value = ((high & 0xFF) << 8) | (low & 0xFF)
        # Check for sign bit and turn into a negative value if set.
        if value & 0x8000 != 0:
            value -= 1 << 16
        return value

    def _read(self, mux, gain, data_rate, mode):
        """Perform an ADC read with the provided mux, gain, data_rate, and mode
        values.  Returns the signed integer result of the read.
        """
        config = ADS1115_CONFIG_OS_SINGLE  # Go out of power-down mode for conversion.
        # Specify mux value.
        config |= (mux & 0x07) << ADS1115_CONFIG_MUX_OFFSET
        # Validate the passed in gain and then set it in the config.
        if gain not in ADS1115_CONFIG_GAIN:
            raise ValueError('Gain must be one of: 2/3, 1, 2, 4, 8, 16')
        config |= ADS1115_CONFIG_GAIN[gain]
        # Set the mode (continuous or single shot).
        config |= mode
        # Get the default data rate if none is specified
        if data_rate is None:
            data_rate = self._data_rate_default()
        # Set the data rate (this is controlled by the subclass as it differs
        config |= self._data_rate_config(data_rate)
        config |= ADS1115_CONFIG_COMP_QUE_DISABLE  # Disble comparator mode.
        # Send the config value to start the ADC conversion.
        # Explicitly break the 16-bit value down to a big endian pair of bytes.
        self._device.writeList(ADS1115_POINTER_CONFIG, [(config >> 8) & 0xFF, config & 0xFF])
        # Wait for sampling to end based on sample rate plus a small offset
        sleep(1.0/data_rate+0.0001)
        # Retrieve the result.
        result = self._device.readList(ADS1115_POINTER_CONVERSION, 2)
        return self._conversion_value(result[1], result[0])

    def readADC(self, channel, gain=2/3, data_rate=None):
        """Read a single ADC channel and return the ADC value as a signed integer
        result.  Channel must be a value within 0-3.
        """
        assert 0 <= channel <= 3, 'Channel must be a value within 0-3!'
        # Perform a single shot read and set the mux value to the channel plus
        return self._read(channel + 0x04, gain, data_rate, ADS1115_CONFIG_MODE_SINGLE)

    def readVolts(self, channel, max_v, fullscale, ):
        """
        """
        assert 0 <= channel <= 3, 'Channel must be a value within 0-3!'
        # Perform a single shot read and set the mux value to the channel plus
        return self.readADC(channel) * max_v / fullscale