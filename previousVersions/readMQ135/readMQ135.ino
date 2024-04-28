int sensorPin = A0;
int val = 0;

#define atmCO2 = 418.51;
float Rl = 22000;
float R0 = 228067;
float minvalue = 80;


void setup() {
  Serial.begin(9600);

}

void loop() {
  val = analogRead(sensorPin);
  float CO2level = val - minvalue;
  CO2level = map(CO2level, 0, 1023, 416.36, 5000);
  float voltage = val*(5.0/1023.0);


//  float Rs = Rl*(5.0/voltage - 1);
//  float ratio = Rs/R0;

  Serial.println(val);
  Serial.println(CO2level);
//  Serial.println(voltage);
//  Serial.println(Rs);
//  Serial.println(ratio);
  delay(1000);

}
