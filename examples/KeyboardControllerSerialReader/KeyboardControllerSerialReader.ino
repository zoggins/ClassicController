//
// KeyboardControllerSerialReader.ino
//
// Author:
//       Christopher J. Mallery <christopher.mallery@gmail.com>
//
// Copyright (c) 2018 Christopher J. Mallery <http://www.zoggins.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <KeyboardController.h>

// Controller DB9 pins (looking face-on to the end of the plug):
//
// 5 4 3 2 1
//  9 8 7 6
//
// Connect pin 7 to +5V and pin 8 to GND
// Connect the remaining pins 1-6,9 to digital I/O pins 2-8 
KeyboardController controller;

// Controller states
word currentState = 0;
word lastState = -1;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    currentState = controller.getState();
    sendState();
}

void sendState()
{
    // Only report controller state if it's changed
    if (currentState != lastState)
    {
        Serial.print((currentState & KC_BTN_ONE)    ? "1" : "-");
        Serial.print((currentState & KC_BTN_TWO)    ? "2" : "0");
        Serial.print((currentState & KC_BTN_THREE)  ? "3" : "0");
        Serial.print((currentState & KC_BTN_FOUR)  ? "4" : "0");
        Serial.print((currentState & KC_BTN_FIVE) ? "5" : "0");
        Serial.print((currentState & KC_BTN_SIX) ? "6" : "0");
        Serial.print((currentState & KC_BTN_SEVEN)     ? "7" : "0");
        Serial.print((currentState & KC_BTN_EIGHT)     ? "8" : "0");
        Serial.print((currentState & KC_BTN_NINE)     ? "9" : "0");
        Serial.print((currentState & KC_BTN_STAR)     ? "*" : "0");
        Serial.print((currentState & KC_BTN_ZERO)     ? "0" : "0");
        Serial.print((currentState & KC_BTN_POUND)     ? "#" : "0");

        Serial.print("\n");
        lastState = currentState;
    }
}
