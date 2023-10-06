//Pot data collection; Malia Hodges; SEED Project 2023

boolean started = false;
int solenoidPin = 4; //output pin 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(solenoidPin, OUTPUT); //sets pin as output
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    Serial.read();
    started = !started;
    Serial.println(started == true ? "collecting" : "stopped");
    if (started == true){
      collect();
    }
  }
}

int collect(){
  int potValue = analogRead(A0);
    Serial.println(potValue);
    delay(100);
    if (potValue > 500){
      digitalWrite(solenoidPin, HIGH); //switch solenoid ON
      delay(1000);
    }
    if (potValue < 500){
      digitalWrite(solenoidPin, LOW); //switch solenoid OFF
      delay(1000);
    }
}