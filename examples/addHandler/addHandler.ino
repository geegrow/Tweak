#include "Tweak.h"
#include <MemoryFree.h>

uint8_t tmpId = 0;
volatile uint16_t counter = 0;

class CallBackOwner : public TweakCallbackOwnerClass {
  uint8_t code;
  public:
  CallBackOwner(uint8_t code){
    this->code = code;
  }

  void tweakHandler() {
    Serial.print("Hi, it's me :-). My code: ");
    Serial.println(this->code);
  }
};

void setup() {
  Serial.begin(9600);
  delay(5000);

  CallBackOwner myObj1 =  CallBackOwner(32);
  CallBackOwner myObj2 =  CallBackOwner(43);

  tmpId = Tweak::attachMsMemberCallback(&myObj1, 10000);
  Serial.print("\nAttached new handler with ID:");
  Serial.println(tmpId);
  Serial.print("freeMemory()="); Serial.println(freeMemory());


  tmpId = Tweak::attachMsMemberCallback(&myObj2, 10000);
  Serial.print("\nAttached new handler with ID:");
  Serial.println(tmpId);
  Serial.print("freeMemory()="); Serial.println(freeMemory());

//  Tweak::detachCallback(1);
//  Serial.println("\nHandler 1 was detached.");
//  Serial.print("freeMemory()="); Serial.println(freeMemory());

  tmpId = Tweak::attachMsCallback(myTimerHandler2, 5000);
  Serial.print("\nAttached new handler with ID:");
  Serial.println(tmpId);
  Serial.print("freeMemory()="); Serial.println(freeMemory());

  tmpId = Tweak::attachMsLambda(
    [](void){
      Serial.print("\nHallo from lambda!!!");
    },
    500
  );
  Serial.print("\nAttached new Lambda with ID:");
  Serial.println(tmpId);
  Serial.print("freeMemory()="); Serial.println(freeMemory());
}

void loop() {

  Serial.print("\nCall from main loop:");
  Serial.println(counter);
  counter = 0;
  Serial.println();
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
  Tweak::delay(1000);
  Serial.println("Call from main loop 2");
  Tweak::delay(1000);
  Serial.println("Call from main loop 3");
  Tweak::delay(1000);
  Serial.println("Call from main loop 4");
  Tweak::delay(1000);
  Serial.println("Call from main loop 5");
}


void myTimerHandler1(void) {
//  Serial.println("myHandler 1");
  int16_t a;
  for (uint8_t i = 0; i < 100; i++) {
     a = sin(random(0,1));
  }
}

void myTimerHandler2(void) {
  Serial.println("myHandler  2");
}

void myTimerHandler3(void) {
  Serial.print("*");
}

void myTimerHandler4(void) {
  counter++;
}
