#ifndef vPOTENTIOMETER_H_
#define vPOTENTIOMETER_H_

/****
 * Visual representation of a potentiometer.
 * TODO:
 * - allow text size to be changed
 * - implement necessary getters
 * - fix text refresh -> what is the parameter size? Look into adafruit doc
 - add default color
*/

/*
  #define INNER_DISC 220
  #define INDICATOR_LENGTH 210*/
#define TEXT_BASE_WIDTH 6 // 5+1 spacing
#define TEXT_BASE_HEIGHT 7
//#define MAX_STRING_LENGTH 10


#include <Oscil.h>   // Mozzi
#include <tables/sin256_int8.h>   // Mozzi
#include <tables/cosphase256_int8.h>   // Mozzi
#include <Adafruit_GFX.h>
//#include "Parameter.h"
#include "GT_Parameter.h"

/** A virtual class for different kinds of potentiometers
 */
template<typename T>
class vPotentiometer  
{
public:
  vPotentiometer(Adafruit_ILI9341* _screen, unsigned long response_time=50): response_time{response_time} 
  {screen = _screen; NBit = sizeof(T)<<3;}

  /** Set the size of the visual potentiometer
      @param _size the new size
  */
  void setSize(int16_t _size) {size = _size;}
  
  /** Set the position of the visual potentiometer
      @param X the X position
      @param Y the Y position
  */
  void setPosition(int16_t X, int16_t Y){pos_X = X; pos_Y = Y;}
  
  /** Set the value displayed by the visual potentiometer
      @param _value the value of depth over full range of the potentiometer
  */
  void setValue(T _value) {value = _value;}

  
  /** Set the value displayed by the visual potentiometer
      @param _value the value of depth over Nbits
      @param inputBit the depth of the inputted value
  */
  template<typename T2>
  void setValue(T2 _value, byte inputNBit=sizeof(T2)<<3) {value = scale<T2,T>(_value,inputNBit, NBit);}
  

  /** Set the color of the visual potentiometer
      @param _color the new color
  */
  void setColor(uint16_t _color) {color = _color;}

  /** Set the background color of the visual potentiometer (ie the color if the potentiometer was not here)
      @param _color the new background color
  */
  void setBackgroundColor(uint16_t _color)  {background_color = _color;}

  /** Probe if a point (X,Y) is touching the object
      @param X the horinzontal position of the point
      @param Y the vertical position of the point
      @return boolean telling if the point is touching the object
  */
  bool isInHitBox(int16_t X, int16_t Y){
    if (X > pos_X - (size) && X <pos_X +(size) && Y>pos_Y - (size) && Y < pos_Y + (size))
      {
	last_hit_time = millis();
	return true;
      }
    else return false;
  }


  /**
     Attach a parameter to the visual potentiometer, and detach any previous ones
     @param _parameter a pointer to a GT_Parameter
  */
  void attachParameter(GT_Parameter * _parameter){parameter = _parameter;}

  /**
     Returns the attached parameter to this visual potentiometer
     @param _parameter a pointer to a GT_Parameter
  */
  /*const*/ GT_Parameter * getAttachedParameter() const {return parameter;}

  /**
     Return the last time isInHitBox returned true
  */
  unsigned long getLastHit() const
  {
    return last_hit_time;
  }

  
  /** Set the size of the text of the potentiometer
      @param _text_size the new text size
  */
  void setTextSize(uint8_t _text_size) {text_size = _text_size;}
  
  void setText(String _text) {text = _text;}

  virtual void drawAll(){};
  virtual void update(){};

protected:
  Adafruit_ILI9341 * screen;
  int16_t pos_X, pos_Y, size;
  uint16_t value, text_size;
  uint16_t color, background_color,default_color = 0xFFFF;
  byte NBit;
  bool visible;
  String text, long_text;
  unsigned long last_update, last_hit_time=0,  prospective_blink_time = 200, last_blink_time=0;
  const unsigned long response_time;
  GT_Parameter * parameter=NULL;
};






/** A classic visual representation of a potentiometer
 */
class ClassicPot: public vPotentiometer<uint8_t>
{
public:
  /** Constructor
      @param _screen is the pointer toward a screen
  */
  ClassicPot(Adafruit_ILI9341* _screen, unsigned long response_time=50): vPotentiometer(_screen){}
  
  /** Set the size of the visual potentiometer
      @param _size the new size
  */
  void setSize(int16_t _size) {
    size = _size;
    max_string_length = (size<<1)/TEXT_BASE_WIDTH;
  }

  void setText(String _text) {
    long_text = _text;
    refresh_text = true;
    //if (long_text.length() > max_string_length)	text = long_text.substring(0,max_string_length-1) + ".";
    if (long_text.length() > max_string_length)	text = long_text.substring(0,max_string_length);
    else text =long_text;
  }

  

  /** Update the display of the visual potentiometer if needed
      @note Timing: around 5-10ms
      @todo Change to get the *actual* value of the parameter, not the raw one.
  */
  void update()
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
		else input_color = default_color;

		if (parameter->getProspectiveInput() != nullptr) prospective_color = parameter->getProspectiveInput()->getColor();
		else prospective_color = default_color;


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
		drawValue();
		drawText();
		refresh_text = false;
	      }
	    if (old_color != color)
	      {
		drawContour();
		drawValue();
		drawText();
		refresh_text = false;
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

	    last_update = millis();  
	  }
      }

    void drawAll() // to be used after a black screen
    {
      drawContour();
      drawValue();
      drawText();
    }


  private:
    /// Members of this implementation
    int16_t old_pos_X, old_pos_Y, old_size;
    uint16_t old_color;  
    uint8_t old_value;
    uint8_t max_string_length;
    bool refresh_text;
    GT_Parameter * old_parameter=nullptr;
  
  

    static const int16_t INNER_DISC=220, INDICATOR_LENGTH=210, INDICATOR_WIDTH=20;


    //// Custom functions
    void drawLineAngle(int16_t x0,int16_t  y0,uint8_t value,int16_t length,uint16_t color)
    {
      uint8_t angle =((value *195) >> 8) +30;
      // SIN and COS table are +/- 7bits, so we need to shift that down
      screen->drawLine(x0,y0,x0 -((length * SIN256_DATA[angle]) >> 7), y0 + ((length * COSPHASE256_DATA[angle]) >> 7),color);
    }

    void drawFatLineAngle(int16_t x0,int16_t  y0,uint8_t value,int16_t length,uint16_t color)
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
  

    void eraseContour(){
      screen->fillCircle(old_pos_X, old_pos_Y, old_size, background_color);
    }

    void drawContour(){
      screen->fillCircle(pos_X, pos_Y, size, color);
      screen->fillCircle(pos_X, pos_Y, (size*INNER_DISC) >> 8, background_color);
    }

    void eraseText(){
      int16_t x1,y1;
      uint16_t w,h;
      screen->getTextBounds(text, old_pos_X - (text.length()*(TEXT_BASE_WIDTH>>1)),old_pos_Y + old_size + (TEXT_BASE_HEIGHT>>1),&x1,&y1,&w,&h);
      screen->fillRect( x1,y1,w,h,background_color);
    }

    void drawText(){
      setText(long_text);	    
      screen->setCursor(pos_X - (text.length()*(TEXT_BASE_WIDTH>>1)), pos_Y + size + (TEXT_BASE_HEIGHT>>1));
      screen->setTextColor(color);
      screen->print(text);
    };

    void eraseValue()
    {
      drawFatLineAngle(old_pos_X,old_pos_Y,old_value,(size*INDICATOR_LENGTH)>>8,background_color);
    }

    void drawValue()
    {
      drawFatLineAngle(pos_X,pos_Y,value,(size*INDICATOR_LENGTH)>>8,color);
    }
  };




#endif
