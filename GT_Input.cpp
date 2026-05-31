#include "GT_Input.h"

#include <Arduino.h>
#include "GT_Parameter.h"
//#include "GT_Menu.h"
//#include <MozziHeadersOnly.h>
// #include <mozzi_analog.h>


void GT_AnalogInput::update()
{
  if (millis() - last_update_time > response_time)
    {
      int32_t tamp_value;// = getAnalogValue();
      if (mozzi_mode) tamp_value = mozziAnalogRead_wrapper(pin);
       else tamp_value = analogRead(pin);	
      if (inverted) tamp_value = max_value - tamp_value;
      if (tamp_value != value)
	{
	  value = tamp_value;
	  if (target != nullptr) target->setValue(value, NBits);
	    	  
	}
      last_update_time = millis();
    }
}






void GT_PhysicalInput::setTarget(GT_Parameter * _target){
    
  if (target!= nullptr) target->disconnectInput(); 
  target=_target;
}

void GT_PhysicalInput::setTargetMenu(GT_Menu * _target){  
  target_menu=_target;
}
  
				
/*
void GT_AnalogInput::update()
{
  if (millis() - last_update_time > response_time)
    {
      int32_t tamp_value;
      if (mozzi_mode) tamp_value = mozziAnalogRead(pin);
      else tamp_value = analogRead(pin);	
      if (inverted) tamp_value = max_value - tamp_value;
      if (tamp_value != value)
	{
	  value = tamp_value;
	  if (target != nullptr) target->setValue(value, NBits);
	    	  
	}
      last_update_time = millis();
    }
}
*/

void GT_RotaryEncoder::update()  {
  if (millis() - last_update_time > response_time)
    {
      
      if (button) button->update();
      long position = encoder->getPosition();
      if (inverted) position = -position;
      if (position != 0)
	{
	  //if (target != NULL) target->setValue(target->getValue()+position, 10);
	  if (target_menu) target_menu->incrementValue(position); // place holder
	  else if (target != nullptr) target->incrementValue(position); // TODO: add acceleration in the matter
	  encoder->setPosition(0);
	}
      last_update_time = millis();
    }
}
