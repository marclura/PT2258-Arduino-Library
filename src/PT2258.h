/**
  @file PT2258.h

  @mainpage PT2258 Arduino Library

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