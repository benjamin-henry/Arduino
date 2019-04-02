#include <TimerOne_V2.h>

TimerOne_V2 Timer1; // on instancie le Timer

int builtin_led = LED_BUILTIN;
int led1 = 6;
int led2 = 10;

void task_0() {
  digitalWrite(builtin_led,!digitalRead(builtin_led)); // quand cette routine est appellée, la led change d'état
}

void task_1() {
  digitalWrite(led1,!digitalRead(led1)); 
}

void task_2() {
  digitalWrite(led2,!digitalRead(led2));
}

void setup()
{
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  Timer1.add_task(task_0, FREQUENCY, 2); // la led sur l'arduino change d'état à 2 HZ, ou toutes les 0.5 secondes
  Timer1.add_task(task_1, FREQUENCY, 4); // la led "led1" change d'état à 4 HZ, ou toutes les 0.25 secondes
  Timer1.add_task(task_2, PERIOD, 2);    // la led "led2" change d'état toutes les 2 secondes, ou à 0.5 Hz
  
  Timer1.begin();
}

void loop()
{}
