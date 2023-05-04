/*
  Sketch:   AuCP_Traffic-Light-With-Transition-Timer.ino
  Created:  1-May-2023
  Author:   MicroBeaut (μB)
  GitHub:   https://github.com/MicroBeaut/Finite-State
*/

#include "FiniteState.h"

#define redLightPin     5
#define yellowLightPin  4
#define greenLightPin   3

uint8_t lightPins[] = {redLightPin, greenLightPin, yellowLightPin}; // Define an array of light pins.
const uint8_t numberOfLights = sizeof(lightPins) / sizeof(uint8_t); // Calculate the number of lights.

/*
  ____________________________________________________________________________________________________________________________________________________
  |  State-Transition Table                                                                                                                           |
  |___________________________________________________________________________________________________________________________________________________|
  |             |       |                   | Next-State  | Next-State  |                 |                       |   Delay-Time    |                 |
  | State       |  Id   | Predicate         |   Fase      |   True      | Process         | Event                 | (milliseconds)  | Timer-Type      |
  |_____________|_______|___________________|_____________|_____________|_________________|_______________________|_________________|_________________|
  | RED         |  0	  | TimePredicate     |      0      |      1      | nullptr         | EventOnActionChanged  |           5,000 | TRANS_TIMER     |
  | GREEN       |  1	  | TimePredicate     |      1      |      2      | nullptr         | EventOnActionChanged  |          10,000 | TRANS_TIMER     |
  | YELLOW      |  2	  | TimePredicate     |      2      |      0      | nullptr         | EventOnActionChanged  |           3,000 | TRANS_TIMER     |
  |_____________|_______|___________________|_____________|_____________|_________________|_______________________|_________________|_________________|
*/

bool TimePredicate(id_t id);              // Predicate (Input)
void EventOnActionChanged(EventArgs e);   // Event State

enum TraficState : id_t {
  RED,
  GREEN,
  YELLOW
};

Transition transitions[] = {
  {nullptr, RED, GREEN, nullptr, EventOnActionChanged, 5000, TRANS_TIMER},      // State-1 - NextF = 0, NextT = 1
  {nullptr, GREEN, YELLOW, nullptr, EventOnActionChanged, 10000, TRANS_TIMER},  // State-2 - NextF = 1, NextT = 2
  {nullptr, YELLOW, RED, nullptr, EventOnActionChanged, 3000, TRANS_TIMER},     // State-3 - NextF = 2, NextT = 0
};
const uint8_t numberOfTransitions = sizeof(transitions) / sizeof(Transition);   // Calculate the number of transitions.

FiniteState finiteStateMachine(transitions, numberOfTransitions);               // Define Finite-State Object

void setup() {
  for (uint8_t index = 0; index < numberOfLights; index ++) {
    pinMode(lightPins[index], OUTPUT);      // Set Pin Mode
    digitalWrite(lightPins[index], LOW);    // Set Light with the LOW state.
  }
  finiteStateMachine.begin(RED);            // FSM begins with Initial Transition Id 0
}

void loop() {
  finiteStateMachine.execute();             // Execute the FSM
}

void EventOnActionChanged(EventArgs e) {
  switch (e.action) {
    case ENTRY:
      digitalWrite(lightPins[e.id], HIGH);  // Set Light with the HIGH state.
      break;
    case EXIT:
      digitalWrite(lightPins[e.id], LOW);   // Set Light with the LOW state.
      break;
  }
}