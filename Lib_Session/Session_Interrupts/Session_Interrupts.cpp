#include "Session_Interrupts.h"

Session_Interrupts::Session_Interrupts(){}
Session_Interrupts::~Session_Interrupts(){}

void Session_Interrupts::add(int pin, void(*isr)(), int mode)
{
	attachInterrupt(digitalPinToInterrupt(pin), isr, mode);
}

void Session_Interrupts::remove(int pin)
{
	detachInterrupt(digitalPinToInterrupt(pin));
}