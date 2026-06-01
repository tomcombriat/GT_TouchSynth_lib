#include "GT_Input.h"
#include "GT_Menu.h"
#include "GT_Parameter.h"
#include "GT_Input.h"

void GT_Menu::start(){
  encoder->setTargetMenu(&*this);
  is_active=true;
}


void GT_Menu::exit(){
  encoder->setTargetMenu(nullptr);
  is_active=false;
}



void GT_MenuParameter::start(GT_Parameter * _parameter){
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

void GT_MenuParameter::update()
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



	  // Increased depth
	  if (current_depth != old_depth)
	    {
	      writeCursor(current_item,old_depth,true);
	      writeCursor(current_item,current_depth);
	      old_depth = current_depth;
	    }
	    
	  // Rotary touched, take action
	  if (increment!=0)
	    {
	      if (current_depth==0)      {
		int8_t new_item= current_item+increment;
		if (new_item >=N_item) new_item = N_item-1;
		if (new_item <0) new_item = 0;

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
	      if (current_depth==1)
	      {
		switch (current_item)
		  {
		  case 1:
		    parameter->incrementValue(increment);
		    break;

		  }
	      }
	      increment = 0;
	    }
	}
    }
}

void GT_MenuParameter::writeRightColumn(uint8_t N, bool BG_color)
{
  screen->setCursor((left_margin<<1)+item_width,N*item_height+top_margin); // two times left margin for curso 
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
