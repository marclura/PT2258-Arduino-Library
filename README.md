# PT2258-Arduino-Library
Arduino Library to control the 6-Channel Electronic Volume Controller PT2258

## Setup

### PT2258 address

(1: connected to VCC, 0: connected to GND)

```
| CODE1 | CODE2 | ADDRESS 8bit | ADDRESS 7bit |
|   0   |   0   |    0x80      |    0x40      |
|   1   |   0   |    0x88      |    0x44      |
|   0   |   1   |    0x84      |    0x42      |
|   1   |   1   |    0x8C      |    0x46      |
```

The Wire library uses addresses with 7bit, so if you perform an I2C scan,
the address will appear as right-shifted form the one specified
above and in the data sheet.
Example: I2C scan finds 0x44 -> this means that the PT2258 has the address 0x88

The default address used by the library is 0x88.

If you need to change it, crete the PT2258 object like this:

`PT2258 pt2258(address);` // where the address is a 8bit address (check the table for conversion)

Example: `PT2258 myPT2258(0x84);`


### Wire connection
The PT2258 is specified to work with a bus clock speed of 100kHz max.

Add `Wire.setClock(100000);` in the sutup before calling the begin() funtion for
the PT2258.


### Mute
The PT2258 has the mute on all the channels activated by default when it does power
up as a safety measure.

When the mute is active, even if the volume is changed, the channels will remain silent.

Remember to deactivate the mute to ear the sound.


### Initial volume
The PT2258 has the volume of all the channels at the maximum when it powers on (and the mute active as well, see "Mute" above).


## Functions

### PT2258 (address)
Create a new PT2258 Datatype declaration. Set the I2C address of the IC address (read the PT2258 address paragraph on top for correct use).

`PT2258(uint8_t address)`

Example: `PT2258 myPT2258(0x88);`


### begin()
Start the I2C communication. Returns 1:successful, 0:connection error

`uint8_t begin(void)`


### attenuation(channel, attenuation)
Set the individual channel attenuation in db. Indicate the channel number form 1 to 6 to set. Attenuation in db from 0 (0db) to 79 (79db)

`void attenuation(uint8_t channel,  uint8_t attenuation)`


### attenuationAll(attenuation)
Set the attenuation of all the channels at once in db. Attenuation in db from 0 (0db) to 79 (79db)

`void attenuationAll(uint8_t attenuation)`


### volume(channel, volume)
Set the individual channel volume. Indicate the channel number form 1 to 6 to set. Volume from 0 (min) to 100 (max)

`void volume(uint8_t channel,  uint8_t volume)`


### volumeAll(volume)
Set the volume of all the channels at once. Volume from 0 (min) to 100 (max)

`void volumeAll(uint8_t volume)`


### mute(boolean)
Mute control for all the channels. No matter the volume, the channels will stay silent. It has to be disabled to hear something. Mute active (1, true) or mute not active (0, false)

`void mute(bool mute)`
