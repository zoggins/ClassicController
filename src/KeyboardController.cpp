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
#include "KeyboardController.h"

KeyboardController::KeyboardController()
{
	// Setup the output pins
    for (byte i = 2; i <= 5; i++)
    {
		pinMode(i, OUTPUT);
		digitalWrite(i, HIGH);
    }

    // Setup input pins	
	for (byte i = 6; i <= 8; i++)
	{
        pinMode(i, INPUT_PULLUP);
	}
	
	_currentState = 0;
}

#define MICROSECOND_NOPS "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"

word KeyboardController::getState()
{
	if (max(millis() - _lastReadTime, 0) < KC_READ_DELAY_MS)
    {
		// Not enough time has elapsed, return previously read state
		return _currentState;
    }
	
    noInterrupts();
	readCycle();
    interrupts();
	
	_lastReadTime = millis();
	
    return _currentState;
}

#define MASK_COLUMN_ONE   0b01000000
#define MASK_COLUMN_TWO   0b00000001
#define MASK_COLUMN_THREE 0b10000000

#define MASK_ROW_ONE   0b00000100
#define MASK_ROW_TWO   0b00001000
#define MASK_ROW_THREE 0b00010000
#define MASK_ROW_FOUR  0b00100000

#define WAIT 500

void KeyboardController::readCycle()
{		
	word currentValue = 0;	
	
	digitalWrite(2, LOW);
	for(int i = 0; i < WAIT; ++i)
	{
		asm volatile(MICROSECOND_NOPS);
	}
	if ((PIND & MASK_COLUMN_ONE) == 0) {currentValue |= KC_BTN_ONE;}
	if ((PINB & MASK_COLUMN_TWO) == 0) {currentValue |= KC_BTN_TWO;}
	if ((PIND & MASK_COLUMN_THREE) == 0) {currentValue |= KC_BTN_THREE;}
	digitalWrite(2, HIGH);
	
	digitalWrite(3, LOW);
	for(int i = 0; i < WAIT; ++i)
	{
		asm volatile(MICROSECOND_NOPS);
	}
	if ((PIND & MASK_COLUMN_ONE) == 0) {currentValue |= KC_BTN_FOUR;}
	if ((PINB & MASK_COLUMN_TWO) == 0) {currentValue |= KC_BTN_FIVE;}
	if ((PIND & MASK_COLUMN_THREE) == 0) {currentValue |= KC_BTN_SIX;}	
	digitalWrite(3, HIGH);
	
	digitalWrite(4, LOW);
	for(int i = 0; i < WAIT; ++i)
	{
		asm volatile(MICROSECOND_NOPS);
	}
	if ((PIND & MASK_COLUMN_ONE) == 0) {currentValue |= KC_BTN_SEVEN;}
	if ((PINB & MASK_COLUMN_TWO) == 0) {currentValue |= KC_BTN_EIGHT;}
	if ((PIND & MASK_COLUMN_THREE) == 0) {currentValue |= KC_BTN_NINE;}	
	digitalWrite(4, HIGH);
	
	digitalWrite(5, LOW);
	for(int i = 0; i < WAIT; ++i)
	{
		asm volatile(MICROSECOND_NOPS);
	}
	if ((PIND & MASK_COLUMN_ONE) == 0) {currentValue |= KC_BTN_STAR;}
	if ((PINB & MASK_COLUMN_TWO) == 0) {currentValue |= KC_BTN_ZERO;}
	if ((PIND & MASK_COLUMN_THREE) == 0) {currentValue |= KC_BTN_POUND;}	
	digitalWrite(5, HIGH);
	
	_currentState = currentValue;
}
