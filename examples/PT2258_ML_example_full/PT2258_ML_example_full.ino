/*
  @file PT2258_ML_example_full.ino

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


#include <PT2258.h>
#include <Wire.h>

// If the address is different from the default 0x88, use this declaration PT2258 pt2258(address);
// where the address corresponds to the 8bit address, so for example 0x84
PT2258 pt2258(0x88);

int _delay(50); // "speed" of the volume and attenuation ramps

void setup() {

  Serial.begin(115200); // start the Serial communication

  Wire.begin(); // start the I2C bus
  //Wire.setClock(100000);  // the PT2258 is specified to work with a bus clock of 100kHz

  if(!pt2258.begin()) { // initialise the PT2258
    Serial.println("PT2258: connection error!");
    while(1) delay(10);
  }

  pt2258.volumeAll(0);  // at the beginning the volume is by default at 100%. Set the desired volume at startup before unmuting next

  pt2258.mute(false); // the mute is active when the device powers up. Unmute it to ear the sound

}

void loop() {

  singleChannelsIncreaseVol();


  decreaseVolAll();


  singleChannelsDecreseAttenuation();


  increaseAttenuationAll();


  toggleMute();

}


void singleChannelsIncreaseVol() {

  // Set volume all to zero
  Serial.println("Volume all channels to 0 ...");
  pt2258.volumeAll(0);

  
  // Increase the volume of every channel individually and in sequence one after the other
  Serial.println("increase the volume of every channel individually and in sequence one after the other ...");

  for(byte ch = 1; ch <= 6; ch++) { // sequence of the channels one after the other

    Serial.print("Channel: ");
    Serial.println(ch);

    for(byte vol = 0; vol <= 100; vol++) {   // gradually increase the volume form 0 to 100
      pt2258.volume(ch, vol);
      delay(_delay);
    }

  }

}


void decreaseVolAll() {

  // Set volume all to max
  Serial.println("Volume all channels to max...");
  pt2258.volumeAll(100);

  // Decrease the volume of all the channels together
  Serial.println("decrease the volume of all the channels together...");

  for(byte vol = 100; vol > 0; vol--) { // gradually decrease the volume form 100 to 0
    pt2258.volumeAll(vol);
    delay(_delay);
  }

}

void singleChannelsDecreseAttenuation() {

  // attenuation to max
  pt2258.attenuationAll(79);  // attenuation goes from 0 (0db, no attenuation) to 79 (79db of attenuation, volume is minimum, to zero)

  // Decrease the attenuation of every channel individually and in sequence one after the other
  Serial.println("decrease the attenuation of every channel individually and in sequence one after the other ...");

  for(byte ch = 1; ch <= 6; ch++) { // sequence of the channels one after the other

    Serial.print("Channel: ");
    Serial.println(ch);

    for(byte att = 79; att > 0; att--) {   // gradually reduce the attenuation from -79db to 0db
      pt2258.attenuation(ch, att);
      delay(_delay);
    }

  }

}


void increaseAttenuationAll() {

  // attenuation to min
  pt2258.attenuationAll(0);  // attenuation goes from 0 (0db, no attenuation, maxiumum volume) to 79 (79db of attenuation, volume is minimum, to zero)

  // Increase the attenuation of all the channels together
  Serial.println("increase the attenuation of all the channels together ...");

  for(byte att = 0; att <= 79; att++) { // gradually decrease the volume form 100 to 0
    pt2258.attenuationAll(att);
    delay(_delay);
  }

}


void toggleMute() {

  // Set volume all at 80%
  Serial.println("Volume all channels 80% ...");

  pt2258.volumeAll(80);
  delay(2000);

  // mute all
  Serial.println("Mute all ...");

  pt2258.mute(true);
  delay(2000);

  // unmute all
  Serial.println("Unmute all ...");

  pt2258.mute(false);
  delay(2000);

}


