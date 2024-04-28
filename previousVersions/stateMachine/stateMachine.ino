//This code is for testing the state machine which will direct the flow of the system

#include <Automaton.h>
#include "Atm_blink.h"

Atm_blink led;

void setup() {
  led.begin( 4, 200 );        // Setup a blink machine
  led.trigger( led.EVT_ON );  // Turn it on
}

void loop() {
  led.cycle();
}