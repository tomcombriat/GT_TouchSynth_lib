#ifndef GT_MENU_H_
#define GT_MENU_H_

//#include "GT_Input.h" // for the encoder
#include <Adafruit_GFX.h> // for the screen
#include <Adafruit_ILI9341.h>
#include "GT_Parameter.h"
#include "GT_Input.h"


/*
TODO:
 - make the base class less specific in order to have the base screen selection as a menu also
 - simplify: menu should not necessarily have access to the screen, or the encoder. Make a main menu that dispatch encoder events: turn, pressed, long press, and starts and stops the menues.
*/

// Forward declaration
class GT_RotaryEncoder;
class GT_Parameter;
class GT_PhysicalInput;

/** GT_Menu is a common mother class for all menus
 */
class GT_Menu
{
 public:
  /**
     Constructor
  */
 GT_Menu(Adafruit_ILI9341* _screen, GT_RotaryEncoder* _encoder, uint8_t _N_item=0, unsigned long response_time=50): response_time{response_time}, screen{_screen}, encoder{_encoder}, N_item{_N_item} {}


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
   void start();/*{
			 encoder->setTargetMenu(&*this);
			 is_active=true;
			 }*/

  /**
     Exits the menu
  */
  virtual void exit();



  virtual void drawAll(bool BG_color=false) {}

  virtual void incrementValue(int16_t inc) {}

  virtual void buttonRelease() {}

 protected:
  Adafruit_ILI9341 * const screen;
  GT_RotaryEncoder * const encoder;
  const unsigned long response_time;
  unsigned long last_update_time;
  bool is_active=false;
  const uint8_t N_item;
  uint8_t current_item = 0;
  int16_t increment=0;
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
 GT_MenuParameter(Adafruit_ILI9341* _screen, GT_RotaryEncoder* _encoder,unsigned long response_time=50): GT_Menu(_screen,_encoder,9,response_time) {}


  void start(GT_Parameter * _parameter);

  void update();
 

 private:
  void writeLeftColumn(uint8_t N, bool BG_color=false)
  {
    screen->setCursor(left_margin,N*item_height+top_margin);
    if (!BG_color) screen->setTextColor(color);
    else screen->setTextColor(background_color);
    screen->setTextSize(text_size);
    switch (N){
    case 0:
      screen->print("  Name:");
      //screen->print(parameter->getName());
      break;
    case 1:
      screen->print("  Value:");
      //screen->print(parameter->getName());
      break;
    case 2:
      screen->print("  Input:");
      break;
    case 3:
      screen->print("  MIDI ch:");
      break;
    case 4:
      screen->print("  MIDI MSB CC:");
      break;
    case 5:
      screen->print("  MIDI MSB Learn");
      break;
    case 6:
      screen->print("  MIDI LSB CC:");
      break;
    case 7:
      screen->print("  MIDI LSB Learn");
      break;
    case 8:
      screen->print("  EXIT");
      break;  
    }
  }

  void writeRightColumn(uint8_t N, bool BG_color=false);
  
    

  void writeCursor(uint8_t row, uint8_t column,bool BG_color=false)
  {
    screen->setCursor(left_margin+column*item_width,row*item_height+top_margin);
    if (!BG_color) screen->setTextColor(color);
    else screen->setTextColor(background_color);
    screen->setTextSize(text_size);
    screen->print(">");
  }

  void incrementValue(int16_t inc)
  {
    increment += inc;
  }

  void buttonRelease()
  {
    if (current_depth==0) current_depth=1;
    else current_depth=0;
  }

    /**
     Set the background color
  */
  void setBackgroundColor(uint16_t _color) {background_color = _color;}

  /**
     Set the  color
  */
  void setColor(uint16_t _color) {color = _color;}


    /**
     Set the height and the width of a single item
     @param _height the new height of an element
     @param _width the new width
  */
  void setItemHeightWidth(uint16_t _height,uint16_t _width)
  {
    item_height = _height;
    item_width = _width;
  }

  void exit();


  GT_Parameter * parameter=nullptr;
  int32_t old_value;
uint16_t background_color=0, color = 65535;
uint8_t current_depth=0, old_depth=0, text_size = 1;
   uint16_t item_height=15, item_width=120,top_margin=20, left_margin=15;
 
  
};





#endif
