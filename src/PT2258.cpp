/*
  @file PT2258.cpp

  This library is to control the 6-Channel Electronic Volume Controller IC PT2258

  Created by Marco Lurati, April 21, 2023
  
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

PT2258::PT2258(uint8_t _address)
{
  address = _address >> 1;   // right-shift one bit because Wire library uses 7bit addresses
}

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

void PT2258::attenuation(uint8_t channel, uint8_t attenuation)
{
  uint8_t c = attenuation;
  uint8_t a = c / 10;
  uint8_t b = c - a * 10;

  PT2258Send(channel_address_10[channel-1] + a, channel_address_1[channel-1] + b);
}

void PT2258::attenuationAll(uint8_t attenuation)
{
  uint8_t c = attenuation;
  uint8_t a = c / 10;
  uint8_t b = c - a * 10;

  PT2258Send(PT2258_CHALL_10 + a, PT2258_CHALL_1 + b);
}

void PT2258::volume(uint8_t channel, uint8_t volume)
{
  uint8_t c = map(volume, 0, 100, 79, 0);
  uint8_t a = c / 10;
  uint8_t b = c - a * 10;

  PT2258Send(channel_address_10[channel-1] + a, channel_address_1[channel-1] + b);
}

void PT2258::volumeAll(uint8_t volume)
{
  uint8_t c = map(volume, 0, 100, 79, 0);
  uint8_t a = c / 10;
  uint8_t b = c - a * 10;

  PT2258Send(PT2258_CHALL_10 + a, PT2258_CHALL_1 + b);
}

void PT2258::mute(bool mute)
{
  Wire.beginTransmission(address);
  Wire.write(PT2258_CHALL_MUTE + mute);
  Wire.endTransmission();
}

void PT2258::PT2258Send(uint8_t a, uint8_t b)
{
  Wire.beginTransmission(address);
  Wire.write(a);
  Wire.write(b);
  Wire.endTransmission();
}