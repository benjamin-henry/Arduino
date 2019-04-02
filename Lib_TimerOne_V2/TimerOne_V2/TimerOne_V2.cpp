/*
 *  Interrupt and PWM utilities for 16 bit Timer1 on ATmega168/328
 *  Original code by Jesse Tane for http://labs.ideo.com August 2008
 *  Modified March 2009 by Jérôme Despatis and Jesse Tane for ATmega328 support
 *  Modified June 2009 by Michael Polli and Jesse Tane to fix a bug in setPeriod() which caused the timer to stop
 *  Modified Oct 2009 by Dan Clemens to work with timer1 of the ATMega1280 or Arduino Mega
 *  Modified April 2012 by Paul Stoffregen
 *  Modified again, June 2014 by Paul Stoffregen
 *
 *  This is free software. You can redistribute it and/or modify it under
 *  the terms of Creative Commons Attribution 3.0 United States License. 
 *  To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/us/ 
 *  or send a letter to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 *
 */

//#include "TimerOne.h"
#include "TimerOne_V2.h"
TimerOne _subTimer;              // preinstatiate

unsigned short TimerOne::pwmPeriod = 0;
unsigned char TimerOne::clockSelectBits = 0;
void (*TimerOne::isrCallback)() = NULL;

// interrupt service routine that wraps a user defined function supplied by attachInterrupt
#if defined(__AVR__)
ISR(TIMER1_OVF_vect)
{
  _subTimer.isrCallback();
}

#elif defined(__arm__) && defined(CORE_TEENSY)
void ftm1_isr(void)
{
  uint32_t sc = FTM1_SC;
  #ifdef KINETISL
  if (sc & 0x80) FTM1_SC = sc;
  #else
  if (sc & 0x80) FTM1_SC = sc & 0x7F;
  #endif
  _subTimer.isrCallback();
}

#endif



































TimerOne_V2 * TimerOne_V2::_instance;

TimerOne_V2::TimerOne_V2()
{
	_us_period = 1000;
	_max_tasks = 5;
	_TasksArray = new GeneralFunction[_max_tasks];
	_FunctionsCounters = new double[_max_tasks];
	_FunctionsPeriods = new double[_max_tasks];
	_tasks_in = 0;
}

TimerOne_V2::~TimerOne_V2()
{
	
}

void TimerOne_V2::begin()
{
	_subTimer.initialize(_us_period);
	_subTimer.attachInterrupt(_isr0);
	_instance = this;
}

void TimerOne_V2::_isr0()
{
	_instance->handleInterrupt();
}

void TimerOne_V2::handleInterrupt()
{
	for(int task = 0; task < _tasks_in; task++){
		if(_TasksArray[task]){
			if(!_FunctionsCounters[task]){
				_FunctionsCounters[task] = _FunctionsPeriods[task];
				_TasksArray[task]();
			}else{_FunctionsCounters[task]--;}
		}		
	}	
}

void TimerOne_V2::add_task(GeneralFunction f, int time_style, double time)
{	
	if(_tasks_in < _max_tasks){
		if(time_style == PERIOD){
			_FunctionsPeriods[_tasks_in] = (time * 1000000 / _us_period) - 1;
			_FunctionsCounters[_tasks_in] = _FunctionsPeriods[_tasks_in];
		}

		if(time_style == FREQUENCY){
			_FunctionsPeriods[_tasks_in] = round(1000000 / _us_period / time) - 1;
			_FunctionsCounters[_tasks_in] = _FunctionsPeriods[_tasks_in];
		}
		_TasksArray[_tasks_in++] = f;
	}
}
