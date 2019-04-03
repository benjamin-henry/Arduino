#ifndef SESSION_INTERRUPTS_H
#define SESSION_INTERRUPTS_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class Session_Interrupts
{
	public:
		Session_Interrupts();
		~Session_Interrupts();

		void add(int pin, void(*isr)(), int mode);
		void remove(int pin);
};

#endif