// M7
// Header for Apple class

/*
  Code which contains all functions for the Apple class. These include getters, setters, and
  parameter modifiers
*/

#ifndef Apple_h
#define Apple_h

#include <Arduino.h>
#include <string>
using std::string;

class Apple {
  public:

    // Default constructor
    Apple();

    Apple(String appleName, int o2high, int o2low, int co2high, int co2low);

    // Setters
    void setOxygenHighLevel(float);
    void setOxygenLowLevel(float);
    void setCarbonHighLevel(float);
    void setCarbonLowLevel(float);
    void setOxygenHighMargin();
    void setOxygenLowMargin();
    void setAppleName(String);

    // Getters
    float getOxygenHighLevel();
    float getOxygenLowLevel();
    float getCarbonHighLevel();
    float getCarbonLowLevel();
    long getOxygenHighMargin();
    long getOxygenLowMargin();
    String getAppleName();

    void modifyAppleParams(String);
  
  private:

    // Fields
    String appleName;
    float oxygenHighLevel;
    float oxygenLowLevel;
    float carbonHighLevel;
    float carbonLowLevel;

    long oxygenHighMargin;
    long oxygenLowMargin;
};


#endif
