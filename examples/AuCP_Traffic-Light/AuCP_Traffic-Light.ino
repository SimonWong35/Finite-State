#include "FiniteState.h"

#define redLightPin     5
#define yellowLightPin  4
#define greenLightPin   3

uint8_t lightPins[] = {redLightPin, greenLightPin, yellowLightPin}; // Define an array of light pins.
const uint8_t numberOfLights = sizeof(lightPins) / sizeof(uint8_t); // Calculate the number of lights.

typedef struct {
  unsigned long delayTime;
  unsigned long startTime;
} Timer;

Timer delayTimes[] = {
  {5000},   // RED Delay Time 5 seconds
  {10000},  // GREEN Delay Time 10 seconds
  {3000},   // YELLOW Delay Time 3 seconds
};

bool InputFunction(id_t id);           // Predicate (Input)
void EventFunction(EventArgs e);  // Event State

Transition transitions[] = {
  {InputFunction, 0, 1, nullptr, EventFunction}, // State-0 -  Current-State = 0, Next-State = 1
  {InputFunction, 1, 2, nullptr, EventFunction}, // State-1 -  Current-State = 1, Next-State = 2
  {InputFunction, 2, 0, nullptr, EventFunction}, // State-2 -  Current-State = 2, Next-State = 0
};
const uint8_t numberOftransitions = sizeof(transitions) / sizeof(Transition); // Calculate the number of transitions.

FiniteState finiteStateMachine(transitions, numberOftransitions);             // Define Finite-State Object

void setup() {
  for (uint8_t index = 0; index < numberOfLights; index ++) {
    pinMode(lightPins[index], OUTPUT);    // Set Pin Mode
    digitalWrite(lightPins[index], LOW);  // Set Light with the LOW state.
  }
  finiteStateMachine.begin(0);            // FSM begins with Initial Transition Id 0
}

void loop() {
  finiteStateMachine.execute();  // Execute the FSM
}

bool InputFunction(id_t id) {
  return (millis() - delayTimes[id].startTime >= delayTimes[id].delayTime); // Determine Time Delay
}

void EventFunction(EventArgs e) {
  switch (e.event) {
    case ENTRY:
      delayTimes[e.id].startTime  = millis(); // Reload start time
      digitalWrite(lightPins[e.id], HIGH);    // Set Light with the HIGH state.
      break;
    case EXIT:
      digitalWrite(lightPins[e.id], LOW);     // Set Light with the LOW state.
      break;
  }
}