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
    void setCarbonHighMargin();
    void setCarbonLowMargin();

    // Getters
    float getOxygenHighLevel();
    float getOxygenLowLevel();
    float getCarbonHighLevel();
    float getCarbonLowLevel();
    float getOxygenHighMargin();
    float getOxygenLowMargin();
    float getCarbonHighMargin();
    float getCarbonLowMargin();
  private:

    // Fields
    string appleName;
    float oxygenHighLevel;
    float oxygenLowLevel;
    float carbonHighLevel;
    float carbonLowLevel;

    float oxygenHighMargin;
    float oxygenLowMargin;
    float carbonHighMargin;
    float carbonLowMargin;
};


#endif
