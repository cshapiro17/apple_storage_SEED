//Code for sprint 2 goal -- mimicking air through a pump into a enclosed environment to test it


//**************************************
//INPUTS
//**************************************
int potent1 = A0;             //input analog pin for potentiometer 1

//**************************************
//OUTPUTS
//**************************************
//LEDS
int LED1 = 5;                 //LED1 digital output pin
int LED2 = 6;                 //LED2 digital output pin
int LED3 = 7;                 //LED3 digital output pin
int LED4 = 8;                 //LED4 digital output pin
//solenoids
int solenoidPin1 = 4;         //solenoid 1 output pin
int solenoidPin2 = 3;         //solenoid 1 output pin
//pumps
int pump1 = 2;                //pump 1 output pin

//**************************************
//Global Variables
//**************************************
unsigned long previousMillis = 0;
int interval = 1000;
static unsigned int state;
int potValue1;
boolean solenoid1;


void setup() {
  // pin classifications
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  pinMode(solenoidPin1,OUTPUT);
  pinMode(solenoidPin2,OUTPUT);
  Serial.begin(9600); //Initialize serial communication with a baud rate of 9600
  delay(500);
  state = 1;
}


void loop(){
  unsigned long currentMillis = millis();

  switch(state){

  
    //reset state
    case 1:
      delay(1000);
      state = 2;
    break;
    
    //pump in air state (open solenoid, turn on pump for X amount of time)
    case 2:
      if ((unsigned long)(currentMillis - previousMillis) >= interval) {
        previousMillis = currentMillis;
        digitalWrite(solenoidPin1,HIGH);
        digitalWrite(pump1,HIGH);
        state = 3;
      }
    break;


    //wait for pump to fill room
    case 3:

    break;


    //turn off pump and solenoid
    case 4:
      digitalWrite(solenoidPin1,LOW);
      digitalWrite(pump1,LOW);
    break;


    //Sense State of room pumped in
    case 5:
      potValue1 = analogRead(potent1);
      state = 6;
    break;


    //log value State of room
    case 6:
      Serial.println(potValue1);
      state = 7;
    break;

    //Evaluate state of room 
    case 7:
        solenoid1 = evaluateSensor(potValue1,highLevel,lowLevel,solenoid1,LED1,LED2);

        //if value still outside of spec, keep room open or open room
        if(solenoid1){
          state = 8
        }
        //if value within spec/back to middle, close room
        else{
          state = 9;
        }

    //(OPTION 1) open room state


    //(OPTION 2) close room state
  }
}


/*evaluateSensor is used to measure the value of the potentiometer and determine if a
 * solenoid should be changed or not.
 * Inputs: potentiometer value, high level threshold, low level threshold, 2 LEDs to mimick
 * Output: Boolean true or false to mimick solenoid 
 */
boolean evaluateSensor(int potValue, int highLevel, int lowLevel, boolean previousSolenoid, int LEDA, int LEDB){
  boolean solenoid = previousSolenoid;
  int highMargin = 550;       //high level it has to come back to in order to close valve
  int lowMargin = 450;        //low level it has to come back to in order to close valve

  //Serial.println("Evaluating Potentiometer Value");
  if(potValue>highLevel){
    digitalWrite(LEDA,HIGH);
    digitalWrite(LEDB,LOW);
    solenoid = true;
  }
  else if(potValue<lowLevel){
    digitalWrite(LEDA,LOW);
    digitalWrite(LEDB,HIGH);
    solenoid = true;
  }
  else if(potValue<highMargin && potValue>lowMargin){
    digitalWrite(LEDA,HIGH);
    digitalWrite(LEDB,HIGH);
    solenoid = false;
  }
  else{
    return previousSolenoid;
  }
  return solenoid;
}