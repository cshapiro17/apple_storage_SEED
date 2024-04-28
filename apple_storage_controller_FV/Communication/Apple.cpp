// M7
// Cpp for Apple class

#include "Apple.h"
using namespace std;

// Default constructor
Apple::Apple() : appleName("Default"), oxygenHighLevel(4), oxygenLowLevel(2), carbonHighLevel(3), carbonLowLevel(1)  {
  
}

Apple::Apple(String appleName, int o2high, int o2low, int co2high, int co2low) {
  setAppleName(appleName);
  setOxygenHighLevel(o2high);
  setOxygenLowLevel(o2low);
  setCarbonHighLevel(co2high);
  setCarbonLowLevel(co2low);
  
  setOxygenHighMargin();
  setOxygenLowMargin();
}

/* Getter for the high level of oxygen for the apple
 * Takes in nothing
 * Returns the level of oxygen as a float
 */
float Apple::getOxygenHighLevel() {
  return oxygenHighLevel;
}

/* Getter for the low level of oxygen for the apple
 * Takes in nothing
 * Returns the level of oxygen as a float
 */
float Apple::getOxygenLowLevel() {
  return oxygenLowLevel;
}

/* Getter for the high level of carbon for the apple
 * Takes in nothing
 * Returns the level of carbon as a float
 */
float Apple::getCarbonHighLevel() {
  return carbonHighLevel;
}

/* Getter for the low level of carbon for the apple
 * Takes in nothing
 * Returns the level of carbon as a float
 */
float Apple::getCarbonLowLevel() {
  return carbonLowLevel;
}

/* Getter for the high margin of oxygen for the apple
 * Takes in nothing
 * Returns the margin of oxygen as a long
 */
long Apple::getOxygenHighMargin(){
  return oxygenHighMargin;
}

/* Getter for the low margin of oxygen for the apple
 * Takes in nothing
 * Returns the margin of oxygen as a long
 */
long Apple::getOxygenLowMargin() {
  return oxygenLowMargin;
}

/* Getter for the name of the apple
 * Takes in nothing
 * Returns a string holding the name of the apple
 */
String Apple::getAppleName() {
  return appleName;
}

/* Setter for the high level of oxygen for the apple
 * Takes in a float which represents the level of oxygen
 * Returns nothing
 */
void Apple::setOxygenHighLevel(float highO2) {
  oxygenHighLevel = highO2;
}

/* Setter for the low level of oxygen for the apple
 * Takes in a float which represents the level of oxygen
 * Returns nothing
 */
void Apple::setOxygenLowLevel(float lowO2) {
  oxygenLowLevel = lowO2;
}

/* Setter for the high level of carbon for the apple
 * Takes in a float which represents the level of carbon
 * Returns nothing
 */
void Apple::setCarbonHighLevel(float highN2) {
  carbonHighLevel = highN2;
}

/* Setter for the low level of carbon for the apple
 * Takes in a float which represents the level of carbon
 * Returns nothing
 */
void Apple::setCarbonLowLevel(float lowN2) {
  carbonLowLevel = lowN2;
}

/* Setter for the high margin of oxygen for the apple
 * Takes in nothing
 * Returns nothing
 */
void Apple::setOxygenHighMargin() {
  oxygenHighMargin = ((oxygenHighLevel + oxygenLowLevel) / 2) + (oxygenHighLevel - (oxygenHighLevel + oxygenLowLevel) / 2) / 3;
}

/* Setter for the low margin of oxygen for the apple
 * Takes in nothing
 * Returns nothing
 */
void Apple::setOxygenLowMargin() {
  oxygenLowMargin = ((oxygenHighLevel + oxygenLowLevel) / 2) - (oxygenHighLevel - (oxygenHighLevel + oxygenLowLevel) / 2) / 3;
}

/* Setter for the name of the apple
 * Takes in a string of the name of the apple
 * Returns nothing
 */
void Apple::setAppleName(String appleNameInput) {
  appleName = appleNameInput;
}

/* Function which uses a string of parameters to update the apple thresholds
 * Takes in a string representing the updated parameters
 * Returns nothing
 */
void Apple::modifyAppleParams(String appleParams) {
  // Parse the messgae and assign the values
  int index = appleParams.indexOf("/");
  String highO2 = appleParams.substring(0, index);
  float highO2f = highO2.toFloat();
  //Serial.print("high O2 ");
  //Serial.println(highO2f);
  setOxygenHighLevel(highO2f);

  String remainingString = appleParams.substring(index + 1, appleParams.length());

  //Serial.println(remainingString);

  index = remainingString.indexOf("/");
  String lowO2 = remainingString.substring(0, index);
  float lowO2f = lowO2.toFloat();
  //Serial.print("low O2 ");
  //Serial.println(lowO2f);
  setOxygenLowLevel(lowO2f);

  remainingString = remainingString.substring(index + 1, remainingString.length());

  //Serial.println(remainingString);

  index = remainingString.indexOf("/");
  String highCO2 = remainingString.substring(0, index);
  float highCO2f = highCO2.toFloat();
  //Serial.print("high cO2 ");
  //Serial.println(highCO2f);
  setCarbonHighLevel(highCO2f);

  String lowCO2 = remainingString.substring(index + 1, remainingString.length());
  float lowCO2f = lowCO2.toFloat();
  //Serial.print("low CO2 ");
  //Serial.println(lowCO2f);
  setCarbonLowLevel(lowCO2f);

  // Recalculate the margins for oxygen
  setOxygenHighMargin();
  setOxygenLowMargin();

}
