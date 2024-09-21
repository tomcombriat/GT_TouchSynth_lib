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
  /**
     Constructor
  */
 GT_Menu(Adafruit_ILI9341* _screen, GT_RotaryEncoder* _encoder, uint8_t _N_item=0, uint8_t _N_selectable_item=0, unsigned long response_time=50): response_time{response_time}, screen{_screen}, encoder{_encoder}, N_item{_N_item}, N_selectable_item{_N_selectable_item} {}


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

  /**
     Set the background color
  */
  void setBackgroundColor(uint16_t _color) {background_color = _color;}

  /**
     Set the  color
  */
  void setColor(uint16_t _color) {color = _color;}


 protected:
  Adafruit_ILI9341 * screen;
  GT_RotaryEncoder * encoder;
  const unsigned long response_time;
  unsigned long last_update_time;
  bool is_active=false;
  uint16_t background_color=0, color = 65535;
  const uint8_t N_item, N_selectable_item;
  uint8_t current_item = 0;
};



/**
   A menu to change the settings of a parameter
*/
class GT_MenuParameter: public GT_Menu
{
 public:
  /**
     Constructor
  */
 GT_MenuParameter(Adafruit_ILI9341* _screen, GT_RotaryEncoder* _encoder,unsigned long response_time=50): GT_Menu(_screen,_encoder,1,1,response_time) {}


  void start(GT_Parameter * _parameter)
  {
    GT_Menu::start();
    parameter=_parameter;
    screen->fillScreen(background_color);
    for (uint8_t i=0;i<N_item;i++) writeLeftColumn(i);
    

  }
 

 private:
  void writeLeftColumn(uint8_t N)
  {
    screen->setCursor(0,N*30);
    screen->setTextColor(color);
    switch (N){
    case 0:
      screen->print("  ");
    screen->print(parameter->getName());
    Serial.println(parameter->getName());
    break;
    }
    

  }

  GT_Parameter * parameter=nullptr;
  
};
#endif
