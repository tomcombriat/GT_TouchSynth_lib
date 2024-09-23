#ifndef GT_MENU_H_
#define GT_MENU_H_

//#include "GT_Input.h" // for the encoder
#include <Adafruit_GFX.h> // for the screen
#include "GT_Parameter.h"

// Forward declaration
class GT_RotaryEncoder;

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
  virtual void start();/*{
			 encoder->setTargetMenu(&*this);
			 is_active=true;
			 }*/

  /**
     Exits the menu
  */
  virtual void exit(){
    encoder->setTargetMenu(nullptr);
    is_active=false;
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

  virtual void drawAll(bool BG_color=false) {}

  virtual void incrementValue(int16_t inc) {}

 protected:
  Adafruit_ILI9341 * const screen;
  GT_RotaryEncoder * const encoder;
  const unsigned long response_time;
  unsigned long last_update_time;
  bool is_active=false;
  uint16_t background_color=0, color = 65535;
  const uint8_t N_item;
  uint8_t current_item = 0, current_depth=0, text_size = 1;
  uint16_t item_height=15, item_width=120,top_margin=20, left_margin=15;
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


  void start(GT_Parameter * _parameter)
  {
    GT_Menu::start();
    parameter=_parameter;
    screen->fillScreen(background_color);
    for (uint8_t i=0;i<N_item;i++)
      {
	writeLeftColumn(i);
	writeRightColumn(i);
      }
    incrementValue(0); 
  }

  void update()
  {
    if (is_active)
      {
	if (millis() - last_update_time > response_time)
	  {
	    last_update_time = millis();

	    // Refresh the displayed value that can be changed by another process
	    if (old_value != parameter->getValue())
	      {
		writeRightColumn(1,true);
		writeRightColumn(1);
		old_value = parameter->getValue();
	      }
	    
	    // Rotary touched, take action
	    if (increment!=0)
	      {
		if (current_depth==0)      {
		  uint8_t new_item= current_item+increment;
		  if (new_item >=N_item) new_item = N_item-1;
		  if (int16_t(current_item) + increment <0) new_item=0;

		  switch (new_item) {

		  case 0:
		    new_item = 1;
		    break;
		  }
		  if (new_item != current_item)
		    {
		      writeCursor(current_item,0,true); // erase old cursor
		      writeCursor(new_item,0);
		      current_item = new_item;
		    }

		}
		increment = 0;
	      }
	  }
      }
  }
 

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

  void writeRightColumn(uint8_t N, bool BG_color=false)
  {
    screen->setCursor(left_margin+item_width,N*item_height+top_margin);
    if (!BG_color) screen->setTextColor(color);
    else screen->setTextColor(background_color);
    screen->setTextSize(text_size);
    switch (N){
    case 0:
      screen->print(parameter->getName());
      break;

    case 1:
      if (BG_color) screen->print(old_value);
      else screen->print(parameter->getValue());
      break;
    case 2:
      if (parameter->getInput()!=nullptr)
	{uint16_t prev_color = color;
	  screen->setTextColor(parameter->getInput()->getColor());
	  screen->print(parameter->getInput()->getName());
	  screen->setTextColor(prev_color);
	}
      break;
    case 3:
      screen->print(parameter->getMidiChannel());
      break;
    case 4:
      screen->print(parameter->getMidiControl1());
      break;
    case 6:
      screen->print(parameter->getMidiControl2());
      break;    
    }
  }

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
  


  GT_Parameter * parameter=nullptr;
  int32_t old_value;


 
  
};



#include "GT_Input.h"

void GT_Menu::start(){
  encoder->setTargetMenu(&*this);
  is_active=true;
}

#endif
