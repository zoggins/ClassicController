//
// SegaControllerSpy.cpp
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
#include "SegaControllerSpy.h"

#define PIND_READ( pin )  (PIND&(1<<(pin)))
#define PINB_READ( pin ) (PINB&(1<<(pin-8)))

SegaControllerSpy::SegaControllerSpy(byte db9_pin_7, byte db9_pin_1, byte db9_pin_2, byte db9_pin_3, byte db9_pin_4, byte db9_pin_6, byte db9_pin_9)
{
    // Set pins
    _selectPin = db9_pin_7;

    _inputPins[0] = db9_pin_1;
    _inputPins[1] = db9_pin_2;
    _inputPins[2] = db9_pin_3;
    _inputPins[3] = db9_pin_4;
    _inputPins[4] = db9_pin_6;
    _inputPins[5] = db9_pin_9;

    // Setup the select pin
    pinMode(_selectPin, INPUT);

    // Setup input pins
    for (byte i = 0; i < SCS_INPUT_PINS; i++)
    {
        pinMode(_inputPins[i], INPUT_PULLUP);
    }

    _currentState = 0;
}

inline int quickDigitalRead(uint8_t pin)
{
	if (pin > 7)
	{
		return PINB_READ(pin - 8);
	}

	return PIND_READ(pin);
}

word SegaControllerSpy::getState()
{

    noInterrupts();
    
    // Clear current state
    _currentState = 0;

     readCycle();
    
    interrupts();

    return _currentState;
}

void SegaControllerSpy::readCycle()
{

	while (quickDigitalRead(_selectPin) != LOW);

	// Check that a controller is connected
    _currentState |= (quickDigitalRead(_inputPins[2]) == LOW && quickDigitalRead(_inputPins[3]) == LOW) * SCS_CTL_ON;
            
    // Check controller is connected before reading A/Start to prevent bad reads when inserting/removing cable
    if (_currentState & SCS_CTL_ON)
    {
        // Read input pins for A, Start
        if (quickDigitalRead(_inputPins[4]) == LOW) { _currentState |= SCS_BTN_A; }
        if (quickDigitalRead(_inputPins[5]) == LOW) { _currentState |= SCS_BTN_START; }
    }
	else
	{
		return;
	}

	while (quickDigitalRead(_selectPin) != HIGH);

       
    if (quickDigitalRead(_inputPins[0]) == LOW) { _currentState |= SCS_BTN_UP; }
    if (quickDigitalRead(_inputPins[1]) == LOW) { _currentState |= SCS_BTN_DOWN; }
    if (quickDigitalRead(_inputPins[2]) == LOW) { _currentState |= SCS_BTN_LEFT; }
    if (quickDigitalRead(_inputPins[3]) == LOW) { _currentState |= SCS_BTN_RIGHT; }
    if (quickDigitalRead(_inputPins[4]) == LOW) { _currentState |= SCS_BTN_B; }
    if (quickDigitalRead(_inputPins[5]) == LOW) { _currentState |= SCS_BTN_C; }
        
	while (quickDigitalRead(_selectPin) != LOW);

    if (quickDigitalRead(_inputPins[0]) == LOW && quickDigitalRead(_inputPins[1]) == LOW);
	{
		while (quickDigitalRead(_selectPin) != HIGH);
           
        // Read input pins for X, Y, Z, Mode
        if (quickDigitalRead(_inputPins[0]) == LOW) { _currentState |= SCS_BTN_Z; }
        if (quickDigitalRead(_inputPins[1]) == LOW) { _currentState |= SCS_BTN_Y; }
        if (quickDigitalRead(_inputPins[2]) == LOW) { _currentState |= SCS_BTN_X; }
        if (quickDigitalRead(_inputPins[3]) == LOW) { _currentState |= SCS_BTN_MODE; }
     
    }
}
