#include <TimerOne_V2.h>


/////////in Object.h/////////
class Object
{
  static Object *_instance;
  public:
    Object();
    ~Object();  
    void speak();
    static void run();
};
////////////////////////////////////END


/////////in Object.cpp/////////
Object* Object::_instance;
Object::Object(){_instance = this;} //ctor
Object::~Object(){} // dtor

void Object::run()
{
  _instance->speak();
}

void Object::speak()
{
  Serial.println("Object speaking!");
}
////////////////////////////////////END



TimerOne_V2 Timer1; // on instancie le Timer

int builtin_led = LED_BUILTIN;

void task_0() {
  digitalWrite(builtin_led,!digitalRead(builtin_led)); // quand cette routine est appellée, la led change d'état
}

Object *obj; // <-- On crée un pointeur de type "Objet"

void setup()
{
  Serial.begin(115200);
  pinMode(builtin_led, OUTPUT);

  obj = new Object();

  Timer1.add_task(obj->run, FREQUENCY, 1);
  Timer1.add_task(task_0, FREQUENCY, 1);
  
  Timer1.begin();
}

void loop()
{}
