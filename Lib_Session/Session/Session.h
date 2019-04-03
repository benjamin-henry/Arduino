#ifndef SESSION_H
#define SESSION_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "Session_Timer.h"
#include "Session_Interrupts.h"

class Session
{
	public:
		Session();
		Session(const char* name);
		~Session();

		void set_name(const char* name);
		const char* get_name();

		bool is_enabled();
		bool is_running();

		//////////////////////Timer
		Session_Timer Timer1;
		void start_timer();
		void stop_timer();

		//////////////////////Hardware Interrupts
		Session_Interrupts Interrupts;
		
	protected:
		const char* _name;
		bool _enabled;
		bool _running;

		
	//private:
		
};
#endif
