//This code was written to be easy to understand.
//Modified to work with Giga R1 and O2Sensor Library
//This code was written in the Arduino 1.8.12 IDE
//An Arduino UNO was used to test this code. transitioned to Arduino GIGA
//This code was last tested 6/2020


#include "O2Sensor.h"                                 //include O2 Sensor library

String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product
float o2;                                             //used to hold a float number that is the o2
int count = 0;

O2Sensor EZO2;                                        //initial CO2 Sensor
float reading;                                        //O2 reading for EZO2 sensor


void setup() {                                        //set up the hardware
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  while(!Serial);
  Serial2.begin(9600);                                //set baud rate for the Serial2 port on the GigaR1 to 9600
  while(!Serial2);
  delay(1000);
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
  EZO2.initialize();                                  //inital setup of O2 sensor
  delay(100);
  Serial.println("done setup");
}

void loop() {                                         //here we go...
  // if(count == 0){
  //   EZO2.initialize();
  // }
  if(count == 3){                                     //every 5 seconds get another reading
    reading = EZO2.getPercent();
    Serial.println(reading);                          //print reading
    count = 0;                                        //reset count
  }

  Serial.println(count);                              //print the count cycle we are on
  delay(1000);                                        //delay 1 second
  count ++;                                           //increment count



  // if (sensor_string_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
  //   Serial.println(sensorstring);                     //send that string to the PC's serial monitor
  //   sensorstring = "";                                //clear the string
  //   sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  // }

}
