#ifndef Apple_h
#define Apple_h

#include <Arduino.h>
#include <string>
using std::string;

class Apple {
  public:

    // Default constructor
    Apple();

    // Setters
    void setOxygenHighLevel(float);
    void setOxygenLowLevel(float);
    void setCarbonHighLevel(float);
    void setCarbonLowLevel(float);
    void setOxygenHighMargin();
    void setOxygenLowMargin();

    // Getters
    float getOxygenHighLevel();
    float getOxygenLowLevel();
    float getCarbonHighLevel();
    float getCarbonLowLevel();
    long getOxygenHighMargin();
    long getOxygenLowMargin();
  
  private:

    // Fields
    string appleName;
    float oxygenHighLevel;
    float oxygenLowLevel;
    float carbonHighLevel;
    float carbonLowLevel;

    long oxygenHighMargin;
    long oxygenLowMargin;
};


#endif
