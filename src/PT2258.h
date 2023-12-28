/**
  @file PT2258.h

  @mainpage PT2258 Arduino Library

  @section intro_sec Introduction

  This library is to control the 6-Channel Electronic Volume Controller IC PT2258

  PT2258 address
  ----------------------------------------------------------------------------------

  (1: connected to VCC, 0: connected to GND)

  | CODE1 | CODE2 | ADDRESS 8bit | ADDRESS 7bit |
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


#ifndef PT2258_h
#define PT2258_h

#include <Arduino.h>

/* channel addresses */
#define PT2258_CLEAR_REGISTER 0b11000000 // 0xC0
#define PT2258_CHALL_1        0b11100000 // 0xE0
#define PT2258_CHALL_10       0b11010000 // 0xD0
#define PT2258_CH3_1          0b00010000 // 0x10
#define PT2258_CH3_10         0b00000000 // 0x00
#define PT2258_CH4_1          0b00110000 // 0x30 
#define PT2258_CH4_10         0b00100000 // 0x20
#define PT2258_CH2_1          0b01010000 // 0x50
#define PT2258_CH2_10         0b01000000 // 0x40
#define PT2258_CH5_1          0b01110000 // 0x70
#define PT2258_CH5_10         0b01100000 // 0x60
#define PT2258_CH1_1          0b10010000 // 0x90
#define PT2258_CH1_10         0b10000000 // 0x80
#define PT2258_CH6_1          0b10110000 // 0xB0
#define PT2258_CH6_10         0b10100000 // 0xA0
#define PT2258_CHALL_MUTE     0b11111000 // 0xF8


class PT2258 {
public:
  
  PT2258(uint8_t address);

  uint8_t begin(void);
  void attenuation(uint8_t channel,  uint8_t attenuation);
  void attenuationAll(uint8_t attenuation);
  void volume(uint8_t channel,  uint8_t volume);
  void volumeAll(uint8_t volume);
  void mute(bool mute);

private:
  /*!
   * @param current - IC address 
   */
  uint8_t address;
  void PT2258Send(uint8_t a, uint8_t b);

};


#endif