#ifndef GT_MENU_H_
#define GT_MENU_H_

#include "GT_Input.h" // for the encoder
#include <Adafruit_GFX.h> // for the screen
#include "GT_Parameter.h"

/** GT_Menu is a common mother class for all menus
 */
class GT_Menu
{
 public:
 GT_Menu(Adafruit_ILI9341* _screen, GT_RotaryEncoder* _encoder,unsigned long response_time=50): response_time{response_time}, screen{_screen}, encoder{_encoder} {}


  /** 
      Update the menu
  */
  virtual void update(){}
  
  /**
     Check if the menu is currently active
     @return is_active
  */
  inline bool isActive() {return is_active;}

  /**
     Start the menu
  */
  virtual void start(){is_active=true;}

 protected:
  Adafruit_ILI9341 * screen;
  GT_RotaryEncoder * encoder;
  const unsigned long response_time;
  bool is_active=false;

};
#endif
