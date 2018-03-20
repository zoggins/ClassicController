// !!!!! THIS DOES NOT WORK !!!!!!!!!!
// !!!!! Still working on this !!!!!!!

//
// KeyboardControllerSpy.h
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

#ifndef KeyboardControllerSpy_h
#define KeyboardControllerSpy_h

enum
{
    KCS_BTN_ONE   = 1,
	KCS_BTN_THREE = 2,
    KCS_BTN_TWO   = 4,
    KCS_BTN_FOUR  = 8,
  	KCS_BTN_SIX   = 16,
	KCS_BTN_FIVE  = 32,
	KCS_BTN_SEVEN = 64,	
	KCS_BTN_NINE  = 128,
	KCS_BTN_EIGHT = 256,
	KCS_BTN_STAR  = 512,
	KCS_BTN_POUND = 1024,
	KCS_BTN_ZERO  = 2048

};

const byte KCS_INPUT_PINS = 7;

const unsigned long KCS_READ_DELAY_MS = 5;

class KeyboardControllerSpy {
    public:
        KeyboardControllerSpy();

        word getState();

    private:
        void readCycle();

		unsigned long _lastReadTime;
		
		byte _lastCompletedRow;
		word _currentTotalState;
        word _currentState[4];
		byte _currentRow;
		
		unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

};

#endif