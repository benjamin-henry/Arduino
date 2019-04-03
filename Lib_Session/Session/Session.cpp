#include "Session.h"
#include "Session_Timer.h"

Session::Session()
{
	/*default ctor*/
	Session("Arduino Session");

}

Session::~Session(){/*defaut dtor*/}

Session::Session(const char* name)
{
	_name = name;
	_enabled = 1;
	_running = 1;
}

void Session::set_name(const char* name){_name = name;}
const char* Session::get_name(){return _name;}

bool Session::is_enabled(){return _enabled;}
bool Session::is_running(){return _running;}


//////////////////////////Timer Section
void Session::start_timer()
{
	Timer1 = Session_Timer("Timer 1");
	Timer1.begin();
	Serial.print(Timer1.get_name());
	Serial.println(" Enabled");
}

void Session::stop_timer()
{

}

