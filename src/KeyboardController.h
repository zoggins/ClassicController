//
// KeyboardController.h
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

#ifndef KeyboardController_h
#define KeyboardController_h

enum
{
    KC_BTN_ONE   = 1,
    KC_BTN_TWO   = 2,
    KC_BTN_THREE = 4,
    KC_BTN_FOUR  = 8,
    KC_BTN_FIVE  = 16,
	KC_BTN_SIX   = 32,
	KC_BTN_SEVEN = 64,	
	KC_BTN_EIGHT = 128,
	KC_BTN_NINE  = 256,
	KC_BTN_STAR  = 512,
	KC_BTN_ZERO  = 1024,
	KC_BTN_POUND = 2048
};

const unsigned long KC_READ_DELAY_MS = 5;

class KeyboardController {
    public:
        KeyboardController();

        word getState();

    private:
        void readCycle();

		unsigned long _lastReadTime;
		
        word _currentState;
};

#endif