#include <Oscil.h>   // Mozzi
#include <tables/sin256_int8.h>   // Mozzi
#include <tables/cosphase256_int8.h>   // Mozzi
#include <Adafruit_GFX.h>
//#include "Parameter.h"
#include "GT_Parameter.h"
#include "GT_vVisualElement.h"



void GT_vClassicPot::update()
  {
    if (millis() - last_update > response_time)
      {
	//if (parameter != NULL) setValue(parameter->getRawValue(),parameter->getNBit());
	if (parameter != NULL)
	  {
	    int32_t in_value = parameter->getValue() + parameter->getBias();
	    setValue(in_value, parameter->getNBits());
	    
	    if (parameter->getInput() != parameter->getProspectiveInput()) // blinking (or else) is needed		  
	      {
		uint16_t input_color, prospective_color;
		if (parameter->getInput() != nullptr) input_color = parameter->getInput()->getColor();
		else input_color = default_color; // default to white

		if (parameter->getProspectiveInput() != nullptr) prospective_color = parameter->getProspectiveInput()->getColor();
		else prospective_color = default_color;  // default to white


		if (parameter->getLastProspectiveChangeTime() > last_blink_time)
		  {
		    color = prospective_color;
		    last_blink_time = millis();
		  }

		    
		else if (millis() - last_blink_time > prospective_blink_time) // change color
		  {   
		    if (color == input_color) color = prospective_color;
		    else color = input_color;
		    last_blink_time = millis();
		  }
			
			
	
		/*	if (color == parameter->getInput()->getColor()) color == parameter->getProspectiveInput()->getColor();
			else color=parameter->getInput()->getColor();*/
		      
	      }		
	    else
	      {
		if (parameter->getInput() != nullptr) color=parameter->getInput()->getColor();
		else color = default_color;
	      }

	    ///// Preselect
	    if (isPreselected())
	      {
		if(millis() - last_blink_time > prospective_blink_time)
		  {
		    if (in_color == background_color) in_color = default_color;
		    else in_color = background_color;
		    last_blink_time = millis();
		    Serial.println(in_color);
		  }
	      }
	    if (selected) in_color = default_color;
	      
	  
	      
	    //else color = default_color;
	    // }
	    if (old_parameter != parameter) setText(parameter->getName());
	  }


	if (old_size != size || old_pos_X != pos_X || old_pos_Y != pos_Y) // refresh everything
	  {
	    eraseContour();
	    eraseValue();
	    eraseText();
	    drawContour();
	    drawInContour();
	    drawValue();
	    drawText();
	    refresh_text = false;
	  }
	if (old_color != color)
	  {
	    drawContour();
	    drawInContour();
	    drawValue();
	    drawText();
	    refresh_text = false;
	  }
	if (old_in_color != in_color)
	  {
	    drawInContour();
	    drawValue();
	  }
	if (old_value != value)
	  {
	    eraseValue();
	    drawValue();
	  }
	if (refresh_text)
	  {
	    eraseText();
	    drawText();
	    refresh_text = false;
	  }
    
	old_pos_X = pos_X;
	old_pos_Y = pos_Y;
	old_value = value;
	old_size = size;
	old_color = color;
	old_parameter = parameter;
	old_in_color = in_color;

	last_update = millis();  
      }
  }



void GT_vClassicPot::drawLineAngle(int16_t x0,int16_t  y0,uint8_t value,int16_t length,uint16_t color)
  {
    uint8_t angle =((value *195) >> 8) +30;
    // SIN and COS table are +/- 7bits, so we need to shift that down
    screen->drawLine(x0,y0,x0 -((length * SIN256_DATA[angle]) >> 7), y0 + ((length * COSPHASE256_DATA[angle]) >> 7),color);
  }

void GT_vClassicPot::drawFatLineAngle(int16_t x0,int16_t  y0,uint8_t value,int16_t length,uint16_t color)
  {
    uint8_t angle =((value *195) >> 8) +30;
    int halfWidth = (size * INDICATOR_WIDTH)>>9;
    // int halfWidth = ((size * INDICATOR_WIDTH)*min(abs(COSPHASE256_DATA[angle]),abs(SIN256_DATA[angle]))) >> 15;
    drawLineAngle( x0,  y0, value, length, color);
    byte drawCount = 0;
    int16_t last_x0=x0,last_y0=y0;
    int i = 1;
    while (drawCount  < halfWidth) // prob : a>>b = -1 quelque soit b et non 0
      {
	int16_t new_x0 = x0+((i*COSPHASE256_DATA[angle])/256);
	int16_t new_y0 = y0+((i*SIN256_DATA[angle])/256);
	if ((new_x0 != last_x0) || (new_x0 != last_y0))
	  {drawCount++;	    
	  }
	if (new_x0 != last_x0)
	  {
	    last_x0 = new_x0;
	    drawLineAngle(last_x0,  last_y0, value, length, color);
	    //drawCount++;
	  }
	if (new_y0 != last_y0)
	  {
	    last_y0 = new_y0;
	    drawLineAngle(last_x0,  last_y0, value, length, color);
	    //drawCount++;
	  }
	i++;
      }
    i=0;
    drawCount=0;
    while (drawCount  < halfWidth) // prob : a>>b = -1 quelque soit b et non 0
      {
	int16_t new_x0 = x0-((i*COSPHASE256_DATA[angle])/256);
	int16_t new_y0 = y0-((i*SIN256_DATA[angle])/256);
	// if ((new_x0 != last_x0) || (new_x0 != last_y0)) drawCount++;
	if (new_x0 != last_x0)
	  {
	    last_x0 = new_x0;
	    drawLineAngle(last_x0,  last_y0, value, length, color);
	    drawCount++;
	  }
	if (new_y0 != last_y0)
	  {
	    last_y0 = new_y0;
	    drawLineAngle(last_x0,  last_y0, value, length, color);
	    drawCount++;
	  }
	i++;
      }
  }


void GT_vClassicPot::eraseText()
{
  int16_t x1,y1;
    uint16_t w,h;
    screen->getTextBounds(text, old_pos_X - (text.length()*(TEXT_BASE_WIDTH>>1)),old_pos_Y + old_size + (TEXT_BASE_HEIGHT>>1),&x1,&y1,&w,&h);
    screen->fillRect( x1,y1,w,h,background_color);
  }


void GT_vClassicPot::drawText()
{
  
    setText(long_text);	    
    screen->setCursor(pos_X - (text.length()*(TEXT_BASE_WIDTH>>1)), pos_Y + size + (TEXT_BASE_HEIGHT>>1));
    screen->setTextColor(color);
    screen->print(text);
  }
