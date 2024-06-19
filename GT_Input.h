#ifndef GT_INPUT_H_
#define GT_INPUT_H_
#include <mozzi_analog.h> // for mozziAnalogRead()
#include "GT_Parameter.h"



/**
   Virtual class to embed the two main types of inputs: rotary encoder and potentiometer
*/

class GT_PhysicalInput
{
public:
  /**
     Constructor
     @param name: the name of the input
     @param color: the physical color of the input, in the Adafruit (6-bit 5-6-5 Color Format)
     @param inverted: if the parameter should be inverted (reversing its direction)
  */
  GT_PhysicalInput(const String name, const uint16_t color, const unsigned long response_time=20, bool inverted=false): name{name}, color{color}, response_time{response_time}, inverted{inverted} {
  }


  /** Get the name of the Input
      @return the name of the Input.
  */
  const String getName(){return name;}

  /** Update the Input (not needed for every specification)
   */
  virtual void update(){}


  /**
     Get the color of the Input, in 5,6,5 color format
  */
  uint16_t getColor() {return color;}

  /**
     Set the target of the Input
  */
  void setTarget(GT_Parameter * _target) {target=_target;}

  /**
Set the inversion state of the Input
  */
  void setInverted(bool _inverted) {inverted=_inverted;}
  

  friend class GT_Parameter;



protected:
  int32_t value;
  const String name;
  const uint16_t color;
  const unsigned long response_time;
  unsigned long last_update_time;
  bool inverted;
  GT_Parameter * target=NULL;


};


class GT_AnalogInput: public GT_PhysicalInput
{
public:

  /** 
      Constructor
      @param mozzi_mode: if true, uses the async mozziAnalogRead( which might show some delays…)
  */
  GT_AnalogInput(const String name, const uint16_t color, const int8_t pin, const uint8_t NBits, const unsigned long response_time=20, const bool mozzi_mode=true, bool inverted=false): GT_PhysicalInput(name, color, response_time, inverted), pin{pin}, NBits{NBits}, mozzi_mode{mozzi_mode}
  {
    pinMode(pin, INPUT);
  }

  void update()
  {
    if (millis() - last_update_time > response_time)
      {
	int32_t tamp_value;
	if (mozzi_mode) tamp_value = mozziAnalogRead(pin);
	if (inverted) tamp_value = max_value - tamp_value;
	else tamp_value = analogRead(pin);
	if (tamp_value != value)
	  {
	    value = tamp_value;
	    if (target != NULL) target->setValue(value, NBits);
	  
	  }
	last_update_time = millis();
      }
  }

  
  


private:
  const int8_t pin;
  const uint8_t NBits;
  const bool mozzi_mode;
  const int32_t max_value = 2 << NBits;
};



#endif
