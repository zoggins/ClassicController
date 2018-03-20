//
// KeyboardController.cpp
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
#include "KeyboardControllerSpy.h"

static int cycle;

KeyboardControllerSpy::KeyboardControllerSpy()
{
    // Setup input pins
    for (byte i = 2; i <= 8; i++)
    {
        pinMode(i, INPUT);
    }
	
	_currentRow = 0;
	_currentState[0] = 0;
	_currentState[1] = 0;
	_currentState[2] = 0;
	_currentState[3] = 0;
	
	_currentTotalState = 0;
}


#define MICROSECOND_NOPS "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
word KeyboardControllerSpy::getState()
{
/* 	if (max(millis() - _lastReadTime, 0) < KCS_READ_DELAY_MS)
    {
        // Not enough time has elapsed, return previously read state
        return _currentState;
    }  */
	
    noInterrupts();
	readCycle();
    interrupts();
    return _currentTotalState;
}

#define MASK_COLUMN_ONE   0b0000000001000000
#define MASK_COLUMN_TWO   0b0000000100000000
#define MASK_COLUMN_THREE 0b0000000010000000

#define MASK_ROW_ONE   0b00000100
#define MASK_ROW_TWO   0b00001000
#define MASK_ROW_THREE 0b00010000
#define MASK_ROW_FOUR  0b00100000

#define WAIT 300

void KeyboardControllerSpy::readCycle()
{		
	
	word currentPins = (PIND | (PINB << 8)) & 0b0000000111111100;
	//_currentTotalState = currentPins;
	///return;
	
	// ROW 1
	if ((currentPins & 0b0000000000000100) != 0x4)
	{
		if (_currentRow == 3)
		{
			_currentTotalState =128;
			// Output Row 4's value
			_currentState[0] = 	currentPins & 0b0000000111000000;	
			_currentRow = 0;
			
		}
		else if (_currentRow == 0)
		{
			_currentTotalState = 1;
			// set temp row 1 values	
			_currentState[0] = 	currentPins & 0b0000000111000000;			
		}
	}
	
	// ROW 2
	else if ((currentPins & 0b0000000000001000) != 0x8)
	{
		if (_currentRow == 0)
		{
			_currentTotalState =256;
			// Output Row 1's value
			//_currentTotalState = (~_currentState[0] & 0b0000000111000000) >> 6 ;
			_currentState[1] = 	currentPins & 0b0000000111000000;	
			_currentRow = 1;
		}
		else if (_currentRow == 1)
		{
			_currentTotalState = 2;
			// set temp row 2 values
			_currentState[1] = 	currentPins & 0b0000000111000000;				
		}
	}
	
	// ROW 3
	else if ((currentPins & 0b0000000000010000) != 0x10)
	{
		if (_currentRow == 1)
		{
			_currentTotalState =512;
			// Output Row 2's value
			_currentState[2] = 	currentPins & 0b0000000111000000;	
			_currentRow = 2;
		}
		else if (_currentRow == 2)
		{
			_currentTotalState = 4;
			// set temp row 3 values
			_currentState[2] = 	currentPins & 0b0000000111000000;	
		}
	}
	
	// ROW 4
	else if ((currentPins & 0b0000000000100000) != 0x20)
	{
		if (_currentRow == 2)
		{
			_currentTotalState =1024;
			// Output Row 3's value
			_currentState[3] = 	currentPins & 0b0000000111000000;	
			_currentRow = 3;
		}
		else if (_currentRow == 3)
		{
			_currentTotalState = 8;
			// set temp row 4 values
			_currentState[3] = 	currentPins & 0b0000000111000000;	
		}		
	}
	//word currentValue = 0;	
	
/* 	// ROW 1
	do {} while ((PIND & MASK_ROW_ONE) != 0 || (PIND & MASK_ROW_TWO) == 0 || (PIND & MASK_ROW_THREE) == 0 || (PIND & MASK_ROW_FOUR) == 0);
	
	for(int i = 0; i < WAIT; ++i)
	{
		asm volatile(MICROSECOND_NOPS);
	}

	if ((PIND & MASK_COLUMN_THREE) == 0) {currentValue = KCS_BTN_THREE;}
	else if ((PINB & MASK_COLUMN_TWO) == 0) {currentValue = KCS_BTN_TWO;}
	else if ((PIND & MASK_COLUMN_ONE) == 0) {currentValue = KCS_BTN_ONE;}
	
	// ROW 2
	do {} while ((PIND & MASK_ROW_ONE) == 0 || (PIND & MASK_ROW_TWO) != 0 || (PIND & MASK_ROW_THREE) == 0 || (PIND & MASK_ROW_FOUR) == 0);
	
		for(int i = 0; i < WAIT; ++i)
	{
		asm volatile(MICROSECOND_NOPS);
	}
	
		     if ((PIND & MASK_COLUMN_THREE) == 0) {currentValue = KCS_BTN_SIX;}
		else if ((PINB & MASK_COLUMN_TWO) == 0) {currentValue = KCS_BTN_FIVE;}
		else if ((PIND & MASK_COLUMN_ONE) == 0) {currentValue = KCS_BTN_FOUR;}
	
	// ROW 3
	do {} while ((PIND & MASK_ROW_ONE) == 0 || (PIND & MASK_ROW_TWO) == 0 || (PIND & MASK_ROW_THREE) != 0 || (PIND & MASK_ROW_FOUR) == 0);
	
		for(int i = 0; i < WAIT; ++i)
	{
		asm volatile(MICROSECOND_NOPS);
	}
	
		     if ((PIND & MASK_COLUMN_THREE) == 0) {currentValue = KCS_BTN_NINE;}
		else if ((PINB & MASK_COLUMN_TWO) == 0) {currentValue = KCS_BTN_EIGHT;}
		else if ((PIND & MASK_COLUMN_ONE) == 0) {currentValue = KCS_BTN_SEVEN;}
	
	
	// ROW 4
	do {} while ((PIND & MASK_ROW_ONE) == 0 || (PIND & MASK_ROW_TWO) == 0 || (PIND & MASK_ROW_THREE) == 0 || (PIND & MASK_ROW_FOUR) != 0);
	
	for(int i = 0; i < WAIT; ++i)
	{
		asm volatile(MICROSECOND_NOPS);
	}
	
		     if ((PIND & MASK_COLUMN_THREE) == 0) {currentValue = KCS_BTN_POUND;}
		else if ((PINB & MASK_COLUMN_TWO) == 0) {currentValue = KCS_BTN_ZERO;}
		else if ((PIND & MASK_COLUMN_ONE) == 0) {currentValue = KCS_BTN_STAR;}
	*/
	//_currentState = currentValue; 
}
