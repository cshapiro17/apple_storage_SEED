const int pressureInput = A0; //select the analog input pin for the pressure transducer
const int pressureZero = 102.4; //analog reading of pressure transducer at 0psi
const int pressureMax = 675.8; //analog reading of pressure transducer at 100psi
const float pressuretransducermaxPSI = 150; //psi value of transducer being used
const int baudRate = 9600; //constant integer to set the baud rate for serial monitor
const int sensorreadDelay = 250; //constant integer to set the sensor read delay in milliseconds

float pressureValue = 0; //variable to store the value coming from the pressure transducer

void setup() //setup routine, runs once when system turned on or reset
{
  Serial.begin(baudRate); //initializes serial communication at set baud rate bits per second

  digitalWrite(52, HIGH);
}

void loop() //loop routine runs over and over again forever
{
  pressureValue = analogRead(pressureInput); //reads value from input pin and assigns to variable
  Serial.println(pressureValue);
  pressureValue = ((pressureValue-pressureZero)*pressuretransducermaxPSI)/(pressureMax-pressureZero); //conversion equation to convert analog reading to psi
  Serial.print(pressureValue, 2); //prints value from previous line to serial
  Serial.println("psi"); //prints label to serial
  delay(sensorreadDelay); //delay in milliseconds between read values
}
