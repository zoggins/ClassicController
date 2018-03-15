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

#define PIND_READ(  pin )  (PIND&(1<<(pin)))
#define PINB_READ( pin ) (PINB&(1<<(pin)))

SegaControllerSpy::SegaControllerSpy(byte db9_pin_7, byte db9_pin_1, byte db9_pin_2, byte db9_pin_3, byte db9_pin_4, byte db9_pin_6, byte db9_pin_9)
{
    // Set pins
    _selectPin = 0;

/*     2 = db9_pin_1;
    3 = db9_pin_2;
    4 = db9_pin_3;
    5 = db9_pin_4;
    6 = db9_pin_6;
    7 = db9_pin_9; */

    // Setup the select pin
    pinMode(_selectPin, INPUT_PULLUP);

    // Setup input pins
    for (byte i = 0; i < SCS_INPUT_PINS; i++)
    {
        pinMode(_inputPins[i], INPUT_PULLUP);
    }

    _currentState = 0;
}

word SegaControllerSpy::getState()
{

    noInterrupts();
    word _lastState = _currentState;

    _currentState = 0;

     readCycle();
    
    interrupts();

    return _currentState == 0 ? _lastState : _currentState;  // Kind of a hack to address the intermitted "disconnect"
}

void SegaControllerSpy::readCycle()
{
	//Serial.println("Entering wait for state 2");
	while (PINB_READ(_selectPin) != LOW && !(PIND_READ(4) == LOW && PIND_READ(5) == LOW && !(PIND_READ(2) == LOW && PIND_READ(3) == LOW))) {}
	//Serial.print("Exiting wait for state 2: ");
    //Serial.println(digitalRead(_selectPin) == LOW ? "0" : "1");

	// Check that a controller is connected
    _currentState |= (PIND_READ(4) == LOW && PIND_READ(5) == LOW && !(PIND_READ(2) == LOW && PIND_READ(3) == LOW)) * SCS_CTL_ON;
            
    // Check controller is connected before reading A/Start to prevent bad reads when inserting/removing cable
    if (_currentState & SCS_CTL_ON)
    {
        // Read input pins for A, Start
        if (PIND_READ(6) == LOW) { _currentState |= SCS_BTN_A; }
        if (PIND_READ(7) == LOW) { _currentState |= SCS_BTN_START; }
    }
	else
	{
		//Serial.println("NoSync");
		return;
	}

	//Serial.println("Entering wait for state 3");
	while (PINB_READ(_selectPin) != HIGH) {}
	//Serial.print("Exiting wait for state 3: ");
    //Serial.println(digitalRead(_selectPin) == LOW ? "0" : "1");
    if (PIND_READ(2) == LOW) { _currentState |= SCS_BTN_UP; }
    if (PIND_READ(3) == LOW) { _currentState |= SCS_BTN_DOWN; }
    if (PIND_READ(4) == LOW) { _currentState |= SCS_BTN_LEFT; }
    if (PIND_READ(5) == LOW) { _currentState |= SCS_BTN_RIGHT; }
    if (PIND_READ(6) == LOW) { _currentState |= SCS_BTN_B; }
    if (PIND_READ(7) == LOW) { _currentState |= SCS_BTN_C; }
        
/* 	//Serial.println("Entering wait for state 4 or 6");
	while (PINB_READ(_selectPin) != LOW) {}
	//Serial.print("Exiting wait for state 4 or 6: ");
    //Serial.println(digitalRead(_selectPin) == LOW ? "0" : "1");
	if(!(PIND_READ(2) == LOW && PIND_READ(3) == LOW && PIND_READ(4) == LOW && PIND_READ(5) == LOW))
	{
		//Serial.println("Entering wait for state 5");
		while (PINB_READ(_selectPin) != HIGH ) {}
		//Serial.print("Exiting wait for state 5: ");
		//Serial.println(PINB_READ(_selectPin) == LOW ? "0" : "1");
		//Serial.println("Entering wait for state 6");
		while (PINB_READ(_selectPin) != LOW) {}
		//Serial.print("Exiting wait for state 6: ");
		//Serial.println(PINB_READ(_selectPin) == LOW ? "0" : "1");
	}  

	while (PINB_READ(_selectPin) != LOW) {}
	if (PIND_READ(2) == LOW && PIND_READ(3) == LOW)
	{

		//Serial.println("Entering wait for state 7");
		while (PINB_READ(_selectPin) != HIGH){}
		//Serial.print("Exiting wait for state 7: ");
		//Serial.println(digitalRead(_selectPin) == LOW ? "0" : "1");
           
        // Read input pins for X, Y, Z, Mode
        if (PIND_READ(2) == LOW) { _currentState |= SCS_BTN_Z; }
        if (PIND_READ(3) == LOW) { _currentState |= SCS_BTN_Y; }
        if (PIND_READ(4) == LOW) { _currentState |= SCS_BTN_X; }
        if (PIND_READ(5) == LOW) { _currentState |= SCS_BTN_MODE; }
     
		while (PINB_READ(_selectPin) != LOW) {}
    }*/
	 
	
}
