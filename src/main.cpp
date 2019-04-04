#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>

// microwave sensor settings
#define interrupPin D8
#define statePin D7
volatile byte interruptCounter;
int numberOfInterrupts = 0;
volatile int sensor_state = LOW;

// declare functions
void handle_movement();

// ticker
Ticker handleMicrowave(handle_movement, 1000);

//--- interrupt handler
void handleInterrupts () {
  interruptCounter++;
}

#define SENSIBILIDAD 10
#define VELOCIDAD_CLICK 150
//---------
void handle_movement () {

  // if the state changed more than once, it means there is a moving object
  // can be adjusted on the situation, equivalent to adjust treshhold of detection speed
  // for moving object
  if (interruptCounter >= SENSIBILIDAD) {
    numberOfInterrupts ++;
    sensor_state = HIGH;
    interruptCounter = 0; // reset de interrupt counter
    yield();

  }else {
    interruptCounter = 0; // if it has not reached the treshold
    sensor_state = LOW;
  }

}

void setup(){

  Serial.begin(115200);
  Serial.println("Setup");
  pinMode(statePin, OUTPUT);
  pinMode(interrupPin, INPUT);
  /// interrupt and ticker settings
  attachInterrupt(digitalPinToInterrupt(interrupPin), handleInterrupts, FALLING);
  handleMicrowave.start();
}


void click(){
  digitalWrite(statePin, HIGH);
  delay(VELOCIDAD_CLICK);
  digitalWrite(statePin, LOW);
  delay(VELOCIDAD_CLICK);
  digitalWrite(statePin, HIGH);
}

//-----------
void loop() {

  Serial.println(interruptCounter);
  handleMicrowave.update();
  digitalWrite(statePin, sensor_state);

  if(sensor_state == HIGH) {
    click();
  }

  yield();
}
