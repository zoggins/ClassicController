//
// ClassicControllerSpySpy.cpp
//
// Author:
//       Christopher Mallery <christopher.mallery@gmail.com>
//
// Copyright (c) 2018 Christopher Mallery <christopher.mallery@gmail.com>
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

#include "Arduino.h"
#include "ClassicControllerSpy.h"

ClassicControllerSpy::ClassicControllerSpy(byte db9_pin_1, byte db9_pin_2, byte db9_pin_3, byte db9_pin_4, byte db9_pin_6, byte db9_pin_9)
{
    // Set pins
    _inputPins[0] = db9_pin_1;
    _inputPins[1] = db9_pin_2;
    _inputPins[2] = db9_pin_3;
    _inputPins[3] = db9_pin_4;
    _inputPins[4] = db9_pin_6;
	_inputPins[5] = db9_pin_9;

    // Setup input pins
    for (byte i = 0; i < CC_INPUT_PINS; i++)
    {
        pinMode(_inputPins[i], INPUT_PULLUP);
    }

    _currentState = 0;
    _lastReadTime = millis();
}

word ClassicControllerSpy::getState()
{
    if (max(millis() - _lastReadTime, 0) < CC_READ_DELAY_MS)
    {
        // Not enough time has elapsed, return previously read state
        return _currentState;
    }
    
    noInterrupts();
    
    // Clear current state
    _currentState = 0;

    readCycle();

    interrupts();
    
    _lastReadTime = millis();

    return _currentState;
}

void ClassicControllerSpy::readCycle()
{
	// Read input pins for Up, Down, Left, Right, 1, 2
	while(_inputPins[2] == LOW && _inputPins[3] == LOW){}
	
	if (digitalRead(_inputPins[0]) == LOW) { _currentState |= CC_BTN_UP; }
	if (digitalRead(_inputPins[1]) == LOW) { _currentState |= CC_BTN_DOWN; }
	if (digitalRead(_inputPins[2]) == LOW) { _currentState |= CC_BTN_LEFT; }
	if (digitalRead(_inputPins[3]) == LOW) { _currentState |= CC_BTN_RIGHT; }
	if (digitalRead(_inputPins[4]) == LOW) { _currentState |= CC_BTN_1; }
	if (digitalRead(_inputPins[5]) == LOW) { _currentState |= CC_BTN_2; }
}
