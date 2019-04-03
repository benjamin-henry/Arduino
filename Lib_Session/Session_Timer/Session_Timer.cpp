///////////////////////////////////////////////////////////TimerOne
///////////////////////////////////////////////////////////TimerOne
///////////////////////////////////////////////////////////TimerOne

// found on github @ https://github.com/PaulStoffregen/TimerOne

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


#include "Session_Timer.h"
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



///////////////////////////////////////////////////////////END
///////////////////////////////////////////////////////////END
///////////////////////////////////////////////////////////END

///////////////////////////////////////////////////////////Session_Timer
///////////////////////////////////////////////////////////Session_Timer
///////////////////////////////////////////////////////////Session_Timer
#include "Session_Timer.h"
Session_Timer * Session_Timer::_instance;
Session_Timer::Session_Timer()
{
	Session_Timer("Timer1");
	_us_period = 1000;
	_Functions = 0;
	_FunctionsCounters = 0;
	_FunctionsPeriods = 0;
	_tasks_in = 0;
	_active = 0;
}

Session_Timer::Session_Timer(const char* name)
{
	_name = name;
	_us_period = 1000;
	_Functions = 0;
	_FunctionsCounters = 0;
	_FunctionsPeriods = 0;
	_tasks_in = 0;
	_active = 0;
}

Session_Timer::~Session_Timer()
{
	
}

void Session_Timer::set_name(const char* name){_name = name;}
const char* Session_Timer::get_name(){return _name;}
void Session_Timer::print_name(){Serial.println(_name);}

void Session_Timer::set_us_period(int us_period)
{	
	_active = 0;
	_subTimer.stop();
	for(int i = 0; i < _tasks_in; i++)
	{
		_FunctionsPeriods[i] *= (double)(_us_period / us_period);
		_FunctionsCounters[i] = _FunctionsPeriods[i];
	}
	_us_period = us_period;
	_subTimer.setPeriod(_us_period);
	_subTimer.restart();
	_active = 1;
}
int Session_Timer::get_us_period()
{
	return _us_period;
}

void Session_Timer::begin()
{
	Serial.println(sizeof(GeneralFunction));
	_active = 1;
	_subTimer.initialize(_us_period);
	_subTimer.attachInterrupt(_isr0);
	_instance = this;
}

void Session_Timer::_isr0()
{
	_instance->handleInterrupt();
}

void Session_Timer::handleInterrupt()
{
	if(_active)
	{
		for(int task = 0; task < _tasks_in; task++){
			//Serial.print(i);
			if(_Functions[task]){
				if(!_FunctionsCounters[task]){
					_FunctionsCounters[task] = _FunctionsPeriods[task];
					_Functions[task]();
				}else{_FunctionsCounters[task]--;}
			}		
		}
	}	
}

void Session_Timer::add(GeneralFunction f, int time_style, double time)
{		
	GeneralFunction *Functions = new GeneralFunction[++_tasks_in];
	double *FunctionsPeriods  = new double[_tasks_in];
	double *FunctionsCounters = new double[_tasks_in];

	for(int i = 0; i < _tasks_in; i++)
	{
		Functions[i] = _Functions[i];
		FunctionsPeriods[i] = _FunctionsPeriods[i];
		FunctionsCounters[i] = _FunctionsCounters[i];
	}

	_Functions = new GeneralFunction[_tasks_in];
	_FunctionsCounters = new double[_tasks_in];
	_FunctionsPeriods = new double[_tasks_in];

	for(int i = 0; i < _tasks_in; i++)
	{
		_Functions[i] = Functions[i];
		_FunctionsPeriods[i] = FunctionsPeriods[i];
		_FunctionsCounters[i] = FunctionsCounters[i];
	}

	delete [] Functions;
	delete [] FunctionsPeriods;
	delete [] FunctionsCounters;

	if(time_style == PERIOD){
		_FunctionsPeriods[_tasks_in-1] = (time * 1000000 / _us_period) - 1;
		_FunctionsCounters[_tasks_in-1] = _FunctionsPeriods[_tasks_in-1];
	}

	if(time_style == FREQUENCY){
		_FunctionsPeriods[_tasks_in-1] = round(1000000 / _us_period / time) - 1;
		_FunctionsCounters[_tasks_in-1] = _FunctionsPeriods[_tasks_in-1];
	}

	_Functions[_tasks_in-1] = f;	
}
///////////////////////////////////////////////////////////END
///////////////////////////////////////////////////////////END
///////////////////////////////////////////////////////////END
