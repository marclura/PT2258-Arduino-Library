/**
  @file PT2258.cpp

  @mainpage PT2258 Arduino Library

  @section intro_sec Introduction

  This library is to control the 6-Channel Electronic Volume Controller IC PT2258

  PT2258 address
  ----------------------------------------------------------------------------------

  (1: connected to VCC, 0: connected to GND)

  | CODE1 | CODE2 | ADDRESS 8bit | ADDRESS 7bit |
  |:-----:|:-----:|:------------:|:------------:|
  |   0   |   0   |    0x80      |    0x40      |
  |   1   |   0   |    0x88      |    0x44      |
  |   0   |   1   |    0x84      |    0x42      |
  |   1   |   1   |    0x8C      |    0x46      |

  The Wire library uses addresses with 7bit, so if you perform an I2C scan,
  the address will appear as right-shifted form the one specified
  above and in the data sheet.
  Example: I2C scan finds 0x44 -> this means that the PT2258 has the address 0x88

  The default address used by the library is 0x88.

  If you need to change it, crete the PT2258 object like this:

  PT2258 pt2258(address); // where the address is a 8bit address (check the table for conversion)

  Example: PT2258 pt2258(0x84);


  Wire connection
  ----------------------------------------------------------------------------------
  The PT2258 is specified to work with a bus clock speed of 100kHz max.

  Add Wire.setClock(100000); in the sutup before calling the begin() funtion for
  the PT2258.


  Mute
  ----------------------------------------------------------------------------------
  The PT2258 has the mute on all the channels activated by default when it does power
  up as a safety measure.

  When the mute is active, even if the volume is changed, the channels will remain silent.

  Remember to deactivate the mute to ear the sound.


  Initial volume
  ----------------------------------------------------------------------------------
  The PT2258 has the volume of all the channels at the maximum when it powers on (and the mute
  active as well, see "Mute" above).

  ----------------------------------------------------------------------------------

  @section author Author

  Created by Marco Lurati, April 21, 2023

  @section license License
  
  MIT License

  Copyright (c) 2023 marclura

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
  
*/

#include <Arduino.h>
#include <Wire.h>
#include "PT2258.h"

uint8_t channel_address_1[6] = {
  PT2258_CH1_1,
  PT2258_CH2_1,
  PT2258_CH3_1,
  PT2258_CH4_1,
  PT2258_CH5_1,
  PT2258_CH6_1
};

uint8_t channel_address_10[6] = {
  PT2258_CH1_10,
  PT2258_CH2_10,
  PT2258_CH3_10,
  PT2258_CH4_10,
  PT2258_CH5_10,
  PT2258_CH6_10
};

/*!
  * @brief PT2258 Datatype declaration Class Constructor
  * 
  * @param address Set the I2C address of the IC address (read the PT2258 address on top for correct use).
  */
PT2258::PT2258(uint8_t _address)
{
  address = _address >> 1;   // right-shift one bit because Wire library uses 7bit addresses
}

/*!
  * @brief Start the I2C communication
  * 
  * @return Return 1:successful, 0:connection error
  */
uint8_t PT2258::begin(void)
{
  //Wire.setClock(100000);  // setting the clock to 100kHz as indicated in the datasheet
  uint8_t return_status = 0;

  Wire.beginTransmission(address);
  Wire.write(PT2258_CLEAR_REGISTER);
  return_status = Wire.endTransmission();

  if(return_status != 0) return_status = 0; // Wire transmission error
  else return_status = 1;

  return return_status;
}

/*!
  * @brief Set the individual channel attenuation in db
  * 
  * @param channel Channel to set, form 1 to 6
  * @param db Attenuation in db from 0 (0db) to 79 (79db)
  */
void PT2258::attenuation(uint8_t channel, uint8_t attenuation)
{
  uint8_t c = attenuation;
  uint8_t a = c / 10;
  uint8_t b = c - a * 10;

  PT2258Send(channel_address_10[channel-1] + a, channel_address_1[channel-1] + b);
}

/*!
  * @brief Set the attenuation of all the channels at once in db
  * 
  * @param db Attenuation in db from 0 (0db) to 79 (79db)
  */
void PT2258::attenuationAll(uint8_t attenuation)
{
  uint8_t c = attenuation;
  uint8_t a = c / 10;
  uint8_t b = c - a * 10;

  PT2258Send(PT2258_CHALL_10 + a, PT2258_CHALL_1 + b);
}

 /*!
  * @brief Set the individual channel volume
  * 
  * @param channel Channel to set, form 1 to 6
  * @param volume Volume from 0 (min) to 100 (max)
  */
void PT2258::volume(uint8_t channel, uint8_t volume)
{
  uint8_t c = map(volume, 0, 100, 79, 0);
  uint8_t a = c / 10;
  uint8_t b = c - a * 10;

  PT2258Send(channel_address_10[channel-1] + a, channel_address_1[channel-1] + b);
}

/*!
  * @brief Set the volume of all the channels at once
  *
  * @param volume Volume from 0 (min) to 100 (max)
  */
void PT2258::volumeAll(uint8_t volume)
{
  uint8_t c = map(volume, 0, 100, 79, 0);
  uint8_t a = c / 10;
  uint8_t b = c - a * 10;

  PT2258Send(PT2258_CHALL_10 + a, PT2258_CHALL_1 + b);
}
/*!
  * @brief Mute control for all the channels. No matter the volume, the channels will stay silent.
  * It has to be disabled to hear something.
  * 
  * @param mute Mute active (1, true) or mute not active (0, false)
  */
void PT2258::mute(bool mute)
{
  Wire.beginTransmission(address);
  Wire.write(PT2258_CHALL_MUTE + mute);
  Wire.endTransmission();
}

/*!
   * @brief Send the datas to the IC
   *
   * @param a 10dB byte value
   * @param b 1dB byte value
   */
void PT2258::PT2258Send(uint8_t a, uint8_t b)
{
  Wire.beginTransmission(address);
  Wire.write(a);
  Wire.write(b);
  Wire.endTransmission();
}