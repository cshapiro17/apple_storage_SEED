#include "Apple.h"
using namespace std;

// Default constructor
Apple::Apple() : appleName("Honeycrisp"), oxygenHighLevel(4), oxygenLowLevel(2), carbonHighLevel(3), carbonLowLevel(1) {
  
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
