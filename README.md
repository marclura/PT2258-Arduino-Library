# PT2258-Arduino-Library

[![license](https://img.shields.io/github/license/peaceiris/actions-gh-pages.svg)](https://github.com/peaceiris/actions-gh-pages/blob/main/LICENSE)
[![Release Version](https://img.shields.io/github/v/release/marclura/PT2258-Arduino-Library?include_prereleases)](https://github.com/marclura/PT2258-Arduino-Library/releases)
[![doxygen](https://img.shields.io/badge/doxygen%20documentation-view-blue)](https://marclura.github.io/PT2258-Arduino-Library)


Arduino Library to control the [6-Channel Electronic Volume Controller PT2258](https://www.princeton.com.tw/%E7%94%A2%E5%93%81%E7%B8%BD%E8%A6%BD/Multimedia-Audio-IC/Electronic-Volume-Controller/Electronic-Volume-Controller-6-Channels)


## Setup

### PT2258 address

(1: connected to VCC, 0: connected to GND)

| CODE1 | CODE2 | ADDRESS 8bit | ADDRESS 7bit |
|:-----:|:-----:|:------------:|:------------:|
|   0   |   0   |    0x80      |    0x40      |
|   1   |   0   |    0x88      |    0x44      |
|   0   |   1   |    0x84      |    0x42      |
|   1   |   1   |    0x8C      |    0x46      |


The Wire library uses addresses with 7bit, so if you perform an I2C scan,
the address will appear as right-shifted from the one specified
above and in the data sheet.
Example: I2C scan finds 0x44 -> this means that the PT2258 has the address 0x88

The default address used by the library is 0x88.

If you need to change it, crete the PT2258 object like this:

`PT2258 pt2258(address);` // where the address is a 8bit address (check the table for conversion)

Example: `PT2258 myPT2258(0x84);`

### Control multiple chips at once

If you need to control more than one chip at once, create multiple instances (objects) of the PT2258 class like this (example with two chips):

```
PT2258 mixerA(0x84);    // CODE1 = 0, CODE2 = 1 (for example)
PT2258 mixerB(0x88);    // CODE1 = 1, CODE2 = 0 (for example)


void setup() {
    Serial.begin(9600);

    Wire.begin(); // start the I2C bus

    if(!mixerA.begin()) { // initialise the PT2258 mixerA chip
        Serial.println("mixerA: connection error!");
        while(1) delay(10);
    }

    if(!mixerB.begin()) { // initialise the PT2258 mixerB chip
        Serial.println("mixerB: connection error!");
        while(1) delay(10);
    }

    mixerA.volumeAll(0);  // at the beginning the volume is by default at 100%. Set the desired volume at startup before unmuting next
    mixerA.mute(false); // the mute is active when the device powers up. Unmute it to ear the sound

    mixerB.volumeAll(0);  // at the beginning the volume is by default at 100%. Set the desired volume at startup before unmuting next
    mixerB.mute(false); // the mute is active when the device powers up. Unmute it to ear the sound

}

void loop() {

    // your code. Control mixerA and mixerB independently. Here an example:

    mixerA.volume(2, 100);

    mixerB.mute(true);

}

```

#### Notes

The maximum amount of PT2258 chips you can connect to the same I2C bus is 4 (because of the four possible addresses that the chips can have). Set them up in order that they have a different address by changing the pin CODE1 and CODE2 (see the PT2258 address table above).

The PT2258 objects you create can be named as you prefer (mixerA and mixerB in the example above). Use names that make more sense to you.
Consider it as defining a new variable where the data type is PT2258; conceptually int my_variable = 0; is the same as saying PT2258 myMixer;


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

## PT2258 (address)
Create a new PT2258 Datatype declaration. Set the I2C address of the IC address (read the PT2258 address paragraph on top for correct use).

`PT2258(uint8_t address)`

Example: `PT2258 myPT2258(0x88);`


## begin()
Start the I2C communication. Returns 1:successful, 0:connection error

`uint8_t begin(void)`

Example: `myPT2258.begin();`


## attenuation(channel, attenuation)
Set the individual channel attenuation in db. Indicate the channel number from 1 to 6 to set. Attenuation in db from 0 (0db) to 79 (79db)

`void attenuation(uint8_t channel,  uint8_t attenuation)`

Example: `myPT2258.attenuation(2, 52);`


## attenuationAll(attenuation)
Set the attenuation of all the channels at once in db. Attenuation in db from 0 (0db) to 79 (79db)

`void attenuationAll(uint8_t attenuation)`

Example: `myPT2258.attenuationAll(12);`


## volume(channel, volume)
Set the individual channel volume. Indicate the channel number from 1 to 6 to set. Volume from 0 (min) to 100 (max)

`void volume(uint8_t channel,  uint8_t volume)`

Example: `myPT2258.volume(3, 85);`


## volumeAll(volume)
Set the volume of all the channels at once. Volume from 0 (min) to 100 (max)

`void volumeAll(uint8_t volume)`

Example: `myPT2258.volumeAll(100);`


## mute(boolean)
Mute control for all the channels. No matter the volume, the channels will stay silent. It has to be disabled to hear something. Mute active (1, true) or mute not active (0, false)

`void mute(bool mute)`

Example: `myPT2258.mute(true);`
