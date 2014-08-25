/*
 Copyright (c) 2013 OpenSourceRF.com.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 Servo.cpp - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
 Copyright (c) 2009 Michael Margolis.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/*
 A servo is activated by creating an instance of the Servo class passing the desired pin to the attach() method.
 The servos are pulsed in the background using the value most recently written using the write() method

 Note that analogWrite of PWM on pins associated with the timer are disabled when the first servo is attached.
 Timers are seized as needed in groups of 12 servos - 24 servos use two timers, 48 servos will use four.

 The methods are:

 Servo - Class for manipulating servo motors connected to Arduino pins.

 attach(pin )  - Attaches a servo motor to an i/o pin.
 attach(pin, min, max  ) - Attaches to a pin setting min and max values in microseconds
 default min is 544, max is 2400

 write()     - Sets the servo angle in degrees.  (invalid angle that is valid as pulse in microseconds is treated as microseconds)
 writeMicroseconds() - Sets the servo pulse width in microseconds
 read()      - Gets the last written servo pulse width as an angle between 0 and 180.
 readMicroseconds()   - Gets the last written servo pulse width in microseconds. (was read_us() in first release)
 attached()  - Returns true if there is a servo attached.
 detach()    - Stops an attached servos from pulsing its i/o pin.

*/

#include "Arduino.h"
#include "Servo.h"

#define usToTicks(_us)    (( clockCyclesPerMicrosecond() * _us) / 8)     // converts microseconds to tick
#define ticksToUs(_ticks) (( (unsigned)_ticks * 8)/ clockCyclesPerMicrosecond() ) // converts from ticks back to microseconds


#define TRIM_DURATION       3                               // compensation ticks to trim adjust for digitalWrite delays (actually 2.6 us)

static servo_t servos[MAX_SERVOS];                          // static array of servo structures

uint8_t ServoCount = 0;                                     // the total number of attached servos

static volatile int8_t Channel[_Nbr_16timers ];             // counter for the servo being pulsed for each timer (or -1 if refresh interval)

// convenience macros
#define SERVO_INDEX_TO_TIMER(_servo_nbr) ((timer16_Sequence_t)(_servo_nbr / SERVOS_PER_TIMER)) // returns the timer controlling this servo
#define SERVO_INDEX_TO_CHANNEL(_servo_nbr) (_servo_nbr % SERVOS_PER_TIMER)       // returns the index of the servo on this timer
#define SERVO_INDEX(_timer,_channel)  ((_timer*SERVOS_PER_TIMER) + _channel)     // macro to access servo index by timer and channel
#define SERVO(_timer,_channel)  (servos[SERVO_INDEX(_timer,_channel)])            // macro to access servo class by timer and channel

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)  // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)  // maximum value in uS for this servo

/************ static functions common to all instances ***********************/


//timer16_Sequence_t timer;

//------------------------------------------------------------------------------
/// Interrupt handlers for the Timers
//------------------------------------------------------------------------------
	void Servo_Handler(timer16_Sequence_t timer, NRF_TIMER_Type *tc, uint8_t channel);
extern "C" {
	#if defined (_useTimer0)
  static void TIMER0_Interrupt(void) {
		Servo_Handler(_timer0, TC_FOR_TIMER0, CHANNEL_FOR_TIMER0);
	}
	#endif
	#if defined (_useTimer1)
  static void TIMER1_Interrupt(void) {
		Servo_Handler(_timer1, TC_FOR_TIMER1, CHANNEL_FOR_TIMER1);
	}
	#endif
	#if defined (_useTimer2)
  static void TIMER2_Interrupt(void) {
		Servo_Handler(_timer2, TC_FOR_TIMER2, CHANNEL_FOR_TIMER2);
	}
	#endif
}

void Servo_Handler(timer16_Sequence_t timer, NRF_TIMER_Type *tc, uint8_t channel)
{
    //static uint32_t timer_counter = 0;
	// clear interrupt
    tc->EVENTS_COMPARE[channel] = 0;
    if (Channel[timer] < 0) {
        tc->TASKS_CLEAR = 1; // channel set to -1 indicated that refresh interval completed so reset the timer
		tc->CC[channel] = 0;
    } else {
        if (SERVO_INDEX(timer,Channel[timer]) < ServoCount && SERVO(timer,Channel[timer]).Pin.isActive == true) {
            digitalWrite(SERVO(timer,Channel[timer]).Pin.nbr, LOW); // pulse this channel low if activated
        }
    }

    Channel[timer]++;    // increment to the next channel
    if( SERVO_INDEX(timer,Channel[timer]) < ServoCount && Channel[timer] < SERVOS_PER_TIMER) {
		tc->CC[channel] += SERVO(timer,Channel[timer]).ticks;
        if(SERVO(timer,Channel[timer]).Pin.isActive == true) {    // check if activated
            digitalWrite( SERVO(timer,Channel[timer]).Pin.nbr,HIGH); // its an active channel so pulse it high
        }
    }
    else {
        // finished all channels so wait for the refresh period to expire before starting over
        if( (tc->CC[channel]) + 4 < usToTicks(REFRESH_INTERVAL) ) { // allow a few ticks to ensure the next OCR1A not missed
            tc->CC[channel] = (unsigned int)usToTicks(REFRESH_INTERVAL);
        }
        else {
            tc->CC[channel] += 4;  // at least REFRESH_INTERVAL has elapsed
        }
        Channel[timer] = -1; // this will get incremented at the end of the refresh period to start again at the first channel
    }
}


static void _initISR(NRF_TIMER_Type *tc, uint32_t channel, uint32_t intenset, IRQn_Type irqn, void (*callback)(void))
{
	// Timer/Counter setup
	tc->TASKS_STOP = 1;
	tc->MODE = TIMER_MODE_MODE_Timer;
	tc->PRESCALER = 3; // Source clock frequency is divided by 2^3 = 8
	tc->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
	tc->TASKS_CLEAR = 1;
	tc->CC[channel] = (uint32_t)(VARIANT_MCK/8/1000); // 1 ms
	tc->EVENTS_COMPARE[channel] = 0;

	// Interrupt setup
	tc->INTENSET = intenset;

	// Enable interrupt
  attachInterrupt(irqn, callback);

	// Start clock
	tc->TASKS_START = 1;
}

static void initISR(timer16_Sequence_t timer)
{
#if defined (_useTimer0)
    if (timer == _timer0)
	{
		//HANDLER_POINTER_FOR_TIMER0 = servo_TIMER0_IRQHandler_Function;
    	_initISR(TC_FOR_TIMER0, CHANNEL_FOR_TIMER0, INTENSET_FOR_TIMER0, IRQn_FOR_TIMER0, TIMER0_Interrupt);
	}
#endif
#if defined (_useTimer1)
    if (timer == _timer1)
	{
    	//HANDLER_POINTER_FOR_TIMER1 = servo_TIMER1_IRQHandler_Function;
		_initISR(TC_FOR_TIMER1, CHANNEL_FOR_TIMER1, INTENSET_FOR_TIMER1, IRQn_FOR_TIMER1, TIMER1_Interrupt);
	}
#endif
#if defined (_useTimer2)
    if (timer == _timer2)
	{
		//HANDLER_POINTER_FOR_TIMER2 = servo_TIMER2_IRQHandler_Function;
    	_initISR(TC_FOR_TIMER2, CHANNEL_FOR_TIMER2, INTENSET_FOR_TIMER2, IRQn_FOR_TIMER2, TIMER2_Interrupt);
	}
#endif
}

static void finISR(timer16_Sequence_t timer)
{
#if defined (_useTimer0)
	if (timer == _timer0)
    	TC_FOR_TIMER0->TASKS_STOP = 1;
#endif
#if defined (_useTimer1)
    if (timer == _timer1)
    	TC_FOR_TIMER1->TASKS_STOP = 1;
#endif
#if defined (_useTimer2)
    if (timer == _timer2)
    	TC_FOR_TIMER2->TASKS_STOP = 1;
#endif
}

static boolean isTimerActive(timer16_Sequence_t timer)
{
  // returns true if any servo is active on this timer
  for(uint8_t channel=0; channel < SERVOS_PER_TIMER; channel++) {
    if(SERVO(timer,channel).Pin.isActive == true)
      return true;
  }
  return false;
}

/****************** end of static functions ******************************/

Servo::Servo()
{
  if (ServoCount < MAX_SERVOS) {
    this->servoIndex = ServoCount++;                    // assign a servo index to this instance
	servos[this->servoIndex].ticks = usToTicks(DEFAULT_PULSE_WIDTH);   // store default values
  } else {
    this->servoIndex = INVALID_SERVO;  // too many servos
  }
}

uint8_t Servo::attach(int pin)
{
  return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max)
{
  timer16_Sequence_t timer;

  if (this->servoIndex < MAX_SERVOS) {
    pinMode(pin, OUTPUT);                                   // set servo pin to output
    servos[this->servoIndex].Pin.nbr = pin;
    this->min  = (MIN_PULSE_WIDTH - min)/4; //resolution of min/max is 4 uS
    this->max  = (MAX_PULSE_WIDTH - max)/4;
    // initialize the timer if it has not already been initialized
    timer = SERVO_INDEX_TO_TIMER(servoIndex);
    if (isTimerActive(timer) == false) {
      initISR(timer);
    }
    servos[this->servoIndex].Pin.isActive = true;  // this must be set after the check for isTimerActive

  }
  return this->servoIndex;
}

void Servo::detach()
{
  timer16_Sequence_t timer;

  servos[this->servoIndex].Pin.isActive = false;
  timer = SERVO_INDEX_TO_TIMER(servoIndex);
  if(isTimerActive(timer) == false) {
    finISR(timer);
  }
}

void Servo::write(int value)
{
  // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
  if (value < MIN_PULSE_WIDTH)
  {
    if (value < 0)
      value = 0;
    else if (value > 180)
      value = 180;

    value = map(value, 0, 180, SERVO_MIN(), SERVO_MAX());
  }
  writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value)
{
  // calculate and store the values for the given channel
  byte channel = this->servoIndex;
  if( (channel < MAX_SERVOS) )   // ensure channel is valid
  {
    if (value < SERVO_MIN())          // ensure pulse width is valid
      value = SERVO_MIN();
    else if (value > SERVO_MAX())
      value = SERVO_MAX();

    value = value - TRIM_DURATION;
    value = usToTicks(value);  // convert to ticks after compensating for interrupt overhead
    servos[channel].ticks = value;
  }
}

int Servo::read() // return the value as degrees
{
  return map(readMicroseconds()+1, SERVO_MIN(), SERVO_MAX(), 0, 180);
}

int Servo::readMicroseconds()
{
  unsigned int pulsewidth;
  if (this->servoIndex != INVALID_SERVO)
    pulsewidth = ticksToUs(servos[this->servoIndex].ticks)  + TRIM_DURATION;
  else
    pulsewidth  = 0;

  return pulsewidth;
}

bool Servo::attached()
{
  return servos[this->servoIndex].Pin.isActive;
}
