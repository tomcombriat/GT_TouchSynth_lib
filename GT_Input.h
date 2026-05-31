#ifndef GT_INPUT_H_
#define GT_INPUT_H_
//#include <mozzi_analog.h> // for mozziAnalogRead()
#include<MozziHeadersOnly.h>
#include "GT_Menu.h"
#include <RotaryEncoder.h>
#include <Button.h>  // from TES_eSax-lib


// Forward declaration
class GT_Parameter;
class GT_Menu;


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
  const inline String getName() const {return name;}

  /** Update the Input (not needed for every specification)
   */
  virtual void update(){};


  /**
     Get the color of the Input, in 5,6,5 color format
  */
  const inline uint16_t getColor() const {return color;}


  /**
     Set the inversion state of the Input
  */
  void setInverted(bool _inverted) {inverted=_inverted;}
  

  friend class GT_Parameter;
  friend class GT_Menu;



protected:
  int32_t value;
  const String name;
  const uint16_t color;
  const unsigned long response_time;
  unsigned long last_update_time;
  bool inverted;
  GT_Parameter * target=nullptr;
  GT_Menu * target_menu = nullptr;

  /**
     Set the target of the Input
  */
   void setTarget(GT_Parameter * _target);
  
  inline void removeTarget(GT_Parameter * _target)
  {
    if (target==_target) target=nullptr;
  }
  

  void setTargetMenu(GT_Menu * _target);



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
  
  void update();


private:
  const int8_t pin;
  const uint8_t NBits;
  const bool mozzi_mode;
  const int32_t max_value = 2 << NBits;
};





class GT_RotaryEncoder: public GT_PhysicalInput
{
public:

  /**
     Constructor
  */
  
  GT_RotaryEncoder(const String name, const uint16_t color, RotaryEncoder *const encoder, Button * const button=nullptr, unsigned long response_time = 20, bool inverted=false): GT_PhysicalInput(name, color, response_time, inverted), encoder{encoder}, button{button} {}

  void update();

    friend class GT_Parameter;
  friend class GT_Menu;
  
protected:
  //void setTargetMenu(GT_Menu * _target);


private:
  RotaryEncoder* const encoder;
  Button * const button;
  //  int32_t value;

};






#endif
