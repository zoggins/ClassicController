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
	_currentState = 0;
	readyToRead = 0;
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
    return _currentState;
}

#define MASK_COLUMN_ONE   0b01000000
#define MASK_COLUMN_TWO   0b00000001
#define MASK_COLUMN_THREE 0b10000000

#define MASK_ROW_ONE   0b00000100
#define MASK_ROW_TWO   0b00001000
#define MASK_ROW_THREE 0b00010000
#define MASK_ROW_FOUR  0b00100000

#define WAIT 300

void KeyboardControllerSpy::readCycle()
{		
	
	word currentValue = 0;	
	
	// ROW 1
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
	
	_currentState = currentValue;
}
