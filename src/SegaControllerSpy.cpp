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

SegaControllerSpy::SegaControllerSpy()
{
    // Setup input pins
	// Assumes pin 8 is SELECT (DB9 pin 7)
	// Assumes pins 2-7 are DB9 pins 1,2,3,4,6,7
	// DB9 pin 5 is +5V
	// DB9 pin 8 is ground
    for (byte i = 2; i <= 8; i++)
    {
        pinMode(i, INPUT_PULLUP);
    }

    _currentState = 0;
}

word SegaControllerSpy::getState()
{

    noInterrupts();
    //word _lastState = _currentState;

    _currentState = 0xFFFF;

     readCycle();
    
    interrupts();

    return ~_currentState; // == 0 ? _lastState : _currentState;  // Kind of a hack to address the intermitted "disconnect"
}

#define MASK_PINS_FOUR_AND_FIVE 0x30
//#define MASK_PINS_FOUR_AND_FIVE 0b0000000000110000
#define MASK_PINS_TWO_AND_THREE 0x0C
//#define MASK_PINS_TWO_AND_THREE 0b0000000000001100
#define MASK_PINS_TWO_THREE_FOUR_FIVE 0x3C
//#define MASK_PINS_FOUR_AND_FIVE 0b0000000000111100

#define TWOC_MASK_A_AND_START_CTRL 0xFE7E
//#define TWOC_MASK_A_AND_START_CTRL 0b1111111001111110
#define TWOC_MASK_UPLRBC 0xFF81
//#define TWOC_MASK_UPLRBC 0b1111111110000001
#define TWOC_MASK_XYXM 0xFFE1
//#define TWOC_MASK_XYXM 0b1111111111100001

#define SHIFT_A_AND_START (TWOC_MASK_A_AND_START_CTRL | (PIND << 1))
#define SHIFT_UDLRBC (TWOC_MASK_UPLRBC | (PIND >> 1))
#define SHIFT_ZYXM (TWOC_MASK_XYXM | (PIND << 7))

#define STATE_TWO (PINB & 1) == 0 && (PIND & MASK_PINS_FOUR_AND_FIVE) == 0 && (PIND & MASK_PINS_TWO_AND_THREE) != 0
#define WAIT_FOR_STATE_TWO (PINB & 1) != 0 || (PIND & MASK_PINS_FOUR_AND_FIVE) != 0 || (PIND & MASK_PINS_TWO_AND_THREE) == 0
#define STATE_THREE (PINB & 1) == 1 && (PIND & MASK_PINS_FOUR_AND_FIVE) != 0
#define WAIT_FOR_STATE_THREE (PINB & 1) != 1 || (PIND & MASK_PINS_FOUR_AND_FIVE) == 0
#define STATE_FOUR_OR_SIX (PINB & 1) == 0 && (PIND & MASK_PINS_FOUR_AND_FIVE) == 0
#define WAIT_FOR_STATE_FOUR_OR_SIX (PINB & 1) != 0 || (PIND & MASK_PINS_FOUR_AND_FIVE) != 0
#define STATE_SIX (PINB & 1) == 0 && (PIND & MASK_PINS_TWO_THREE_FOUR_FIVE) == 0
#define WAIT_FOR_STATE_SIX (PINB & 1) != 0 || (PIND & MASK_PINS_TWO_THREE_FOUR_FIVE) != 0
#define NOT_STATE_SIX (PIND & MASK_PINS_TWO_THREE_FOUR_FIVE) != 0
#define STATE_SEVEN (PINB & 1) == 1 && (PIND & MASK_PINS_TWO_THREE_FOUR_FIVE) != 0
#define WAIT_FOR_STATE_SEVEN (PINB & 1) != 1 || (PIND & MASK_PINS_TWO_THREE_FOUR_FIVE) != 0

void SegaControllerSpy::readCycle()
{
	do {
	} while (WAIT_FOR_STATE_TWO);
	_currentState &= SHIFT_A_AND_START;

	do {
	} while (WAIT_FOR_STATE_THREE);
	_currentState &= SHIFT_UDLRBC;
 
	// Six Button
/* 	do {
		pind = PIND;
	} while (WAIT_FOR_STATE_FOUR_OR_SIX);
	
	if (NOT_STATE_SIX)
	{
		_currentState &= SHIFT_A_AND_START;

		do {
			pind = PIND;
		} while (WAIT_FOR_STATE_THREE);
		_currentState &= SHIFT_UDLRBC;
			
		do {
			pind = PIND;
		} while (WAIT_FOR_STATE_SIX);		
	}
	
	do {
		pind = PIND;
	} while (WAIT_FOR_STATE_SEVEN);		
	_currentState &= SHIFT_ZYXM;
	
	while (WAIT_FOR_STATE_EIGHT){} */
	
}
