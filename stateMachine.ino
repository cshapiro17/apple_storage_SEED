//This code is for testing the state machine which 

#include <StateMachine.h>

const int STATE_DELAY = 1000;
int randomState = 0;
const int LED = 13;

StateMachine machine = StateMachine();

State* S0 = machine.addState(&state0); 
State* S1 = machine.addState(&state1);
State* S2 = machine.addState(&state2);
State* S3 = machine.addState(&state3);
State* S4 = machine.addState(&state4);
State* S5 = machine.addState(&state5);

void setup() {
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  randomSeed(A0);

  S0->addTransition(&transitionS0,S0);    // Transition to itself (see transition logic for details)
  S1->addTransition(&transitionS1S2,S2);  // S1 transition to S2
  S2->addTransition(&transitionS2S3,S3);  // S2 transition to S3
  S3->addTransition(&transitionS3S4,S4);  // S3 transition to S4
  S4->addTransition(&transitionS4S5,S5);  // S4 transition to S5
  S5->addTransition(&transitionS5S0,S0);  // S5 transition to S0
  S5->addTransition(&transitionS5S2,S2);  // S5 transition to S2
}

void loop() {
  machine.run();
  delay(STATE_DELAY);
}