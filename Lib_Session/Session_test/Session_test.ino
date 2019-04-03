#include <Session.h>


/////////in Object.h/////////
class Object : public Session
{
  protected:
    static Object *_instance;

  public:
    Object();
    Object(const char* name);
    ~Object();
    void speak();
    static void run();
};
////////////////////////////////////END


/////////in Object.cpp/////////
Object* Object::_instance;

Object::Object() {
  _instance = this; //ctor
}

Object::Object(const char* name) {
  _instance = this; //ctor
  _name = name;
}

Object::~Object() {} // dtor

void Object::run()
{
  _instance->speak();
}

void Object::speak()
{
  Serial.println("Object speaking!");
}

void callback()
{
  Serial.println("callback1");
}


Session session;
Object obj;

void setup() { 
  Serial.begin(115200);
  session.Timer1.begin();
  session.Timer1.add(obj.run, FREQUENCY, 1);
  session.Interrupts.add(2, callback, FALLING);
}

void loop() {
  
}
