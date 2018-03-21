//
// PaddleSpy.h
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

#ifndef PaddleSpy_h
#define PaddleSpy_h

const byte PS_INPUT_PINS = 2;

const unsigned long PS_READ_DELAY_MS = 5;

enum
{
    PS_BTN_ONE     = 1,
	PS_BTN_TWO 	   = 2,
    PS_PADDLE_ONE  = 4,
    PS_PADDLE_TWO  = 64
};

// This class can be used as both a passive spy and as an active controller.  The only difference is you need to 
// connect DB9 pin 8 to Ardunio Ground and DB9 pin 7 to +5V.  DO NOT connect power if you are passively spying. 

class PaddleSpy {
    public:
        PaddleSpy(byte db9_pin_3, byte db9_pin_4, byte db9_pin_5, byte db9_pin_9);

        unsigned long getState();

    private:
        void readCycle();

        unsigned long _currentState;

        unsigned long _lastReadTime;

        byte _digitalInputPins[PS_INPUT_PINS];
		byte _analogInputPins[PS_INPUT_PINS];
};

#endif
