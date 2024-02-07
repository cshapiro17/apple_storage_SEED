#include "Apple.h"
using namespace std;

// Default constructor
Apple::Apple() : appleName(""), oxygenHighLevel(0), oxygenLowLevel(0), carbonHighLevel(0), carbonLowLevel(0) {
  
}

// Getters
float Apple::getOxygenHighLevel() {
  return oxygenHighLevel;
}
float Apple::getOxygenLowLevel() {
  return oxygenLowLevel;
}
float Apple::getCarbonHighLevel() {
  return carbonHighLevel;
}
float Apple::getCarbonLowLevel() {
  return carbonLowLevel;
}
long Apple::getOxygenHighMargin(){
  return oxygenHighMargin;
}
long Apple::getOxygenLowMargin() {
  return oxygenLowMargin;
}

// Setters
void Apple::setOxygenHighLevel(float highO2) {
  oxygenHighLevel = highO2;
}
void Apple::setOxygenLowLevel(float lowO2) {
  oxygenLowLevel = lowO2;
}
void Apple::setCarbonHighLevel(float highN2) {
  carbonHighLevel = highN2;
}
void Apple::setCarbonLowLevel(float lowN2) {
  carbonLowLevel = lowN2;
}
void Apple::setOxygenHighMargin() {
  oxygenHighMargin = ((oxygenHighLevel + oxygenLowLevel) / 2) + (oxygenHighLevel - (oxygenHighLevel + oxygenLowLevel) / 2) / 3;
}
void Apple::setOxygenLowMargin() {
  oxygenLowMargin = ((oxygenHighLevel + oxygenLowLevel) / 2) - (oxygenHighLevel - (oxygenHighLevel + oxygenLowLevel) / 2) / 3;
}
