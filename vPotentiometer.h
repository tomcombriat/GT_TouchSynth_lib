#ifndef vPOTENTIOMETER_H_
#define vPOTENTIOMETER_H_

/****
 * Visual representation of a potentiometer.
 */


#define INNER_DISC 220
#define INDICATOR_LENGTH 210
#define TEXT_BASE_SIZE 6
//#define MAX_STRING_LENGTH 10


#include <Oscil.h>   // Mozzi
#include <tables/sin256_int8.h>   // Mozzi
#include <tables/cosphase256_int8.h>   // Mozzi
#include <Adafruit_GFX.h>

/** A visual representation of a potentiometer
 */
class vPotentiometer
{
 public:
  /** Constructor
      @param _screen is the pointer toward a screen
  */
  vPotentiometer(Adafruit_ILI9341* _screen){
    screen = _screen;
  }
  
  /** Set the size of the visual potentiometer
      @param _size the new size
  */
  void setSize(int16_t _size) {
    old_size = size;
    size = _size;
    max_string_length = (size<<1)/TEXT_BASE_SIZE;
  }

  /** Set the position of the visual potentiometer
      @param X the X position
      @param Y the Y position
  */
  void setPosition(int16_t X, int16_t Y){
    old_pos_X = pos_X;
    old_pos_Y = pos_Y;
    pos_X = X; pos_Y = Y;
  }

  /** Set the value displayed by the visual potentiometer
      @param _value the displayed value (in 8 bits)
  */
  void setValue(uint8_t _value) {
    old_value = value;
    value = _value;
  }

  /** Set the value displayed by the visual potentiometer
      @param _value the value of depth over 8bits
      @param inputBit the depth of the inputted value
  */
  template<typename T>
    void setValue(T _value, byte inputBit)
    {
      value = _value >> (inputBit - NBit);
    }

  /** Set the color of the visual potentiometer
      @param _color the new color
  */
  void setColor(uint16_t _color) {color = _color;}

  

  /** Set the background color of the visual potentiometer (ie the color if the potentiometer was not here)
      @param _color the new background color
  */
  void setBackgroundColor(uint16_t _color)  {background_color = _color;}

  void setText(String _text) {
    refresh_text = true;
    	if (_text.length() > max_string_length)
	  {
	    text = _text.substring(0,max_string_length-1) + ".";
	  }
	else text = _text;
  }
  

  /** Update the display of the visual potentiometer if needed
      @note Timing: around 5-10ms
  */
  void update()
  {
    /// TODO: PUT REPETITIONS IN FUNCTION
    if (old_size != size)
      {
	screen->fillCircle(old_pos_X, old_pos_Y, old_size, background_color);
	screen->fillRect(old_pos_X - ((max_string_length*TEXT_BASE_SIZE)>>1), old_pos_Y + ((size*700)>>9), max_string_length*TEXT_BASE_SIZE, TEXT_BASE_SIZE+1,background_color);
	refresh_text = true;
	screen->fillCircle(pos_X, pos_Y, size, color);	
	screen->fillCircle(pos_X, pos_Y, (size*INNER_DISC) >> 8, background_color);
	drawLineAngle(pos_X,pos_Y,value,(size*INDICATOR_LENGTH)>>8,color);
	old_pos_X = pos_X;
	old_pos_Y = pos_Y;
	old_value = value;
	old_size = size;
      }
    if (old_pos_X != pos_X || old_pos_Y != pos_Y)
      {
	screen->fillCircle(old_pos_X, old_pos_Y, size, background_color);
	screen->fillCircle(pos_X, pos_Y, size, color);
	screen->fillCircle(pos_X, pos_Y, (size*INNER_DISC) >> 8, background_color);
	drawLineAngle(pos_X,pos_Y,value,(size*INDICATOR_LENGTH)>>8,color);
	old_pos_X = pos_X;
	old_pos_Y = pos_Y;
	old_value = value;
      }
    else if (old_value != value)
      {
	drawLineAngle(pos_X,pos_Y,old_value,(size*INDICATOR_LENGTH)>>8,background_color);
	drawLineAngle(pos_X,pos_Y,value,(size*INDICATOR_LENGTH)>>8,color);
	old_value = value;	     
      }
    else if (old_color != color)
      {
	screen->fillCircle(pos_X, pos_Y, size, color);
	screen->fillCircle(pos_X, pos_Y, (size*INNER_DISC) >> 8, background_color);
	drawLineAngle(pos_X,pos_Y,value,(size*INDICATOR_LENGTH)>>8,color);
	screen->setCursor(pos_X - (text.length()*(TEXT_BASE_SIZE>>1)), pos_Y + ((size*700)>>9));
	screen->setTextColor(color);
	screen->print(text);
	old_color = color;
      }

    if (refresh_text)
      {
	screen->fillRect(pos_X - ((max_string_length*TEXT_BASE_SIZE)>>1), pos_Y + ((size*700)>>9), max_string_length*TEXT_BASE_SIZE, TEXT_BASE_SIZE+1,background_color);
	  
	screen->setCursor(pos_X - (text.length()*(TEXT_BASE_SIZE>>1)), pos_Y + ((size*700)>>9));
	screen->setTextColor(color);
	screen->print(text);
	refresh_text = false;
      }
  }



 private:
  int16_t pos_X, pos_Y, size;
  int16_t old_pos_X, old_pos_Y, old_size;
  uint16_t color, old_color, background_color;
  uint8_t value;
  uint8_t old_value;
  uint8_t max_string_length;
  static const byte NBit=8;
  Adafruit_ILI9341 * screen;
  bool visible;
  String text;
  bool refresh_text;

  void drawLineAngle(int16_t x0,int16_t  y0,uint8_t angle,int16_t length,uint16_t color)
  {
    angle =((uint16_t(angle) *195) >> 8) +30;
    screen->drawLine(x0,y0,x0 -((length * SIN256_DATA[angle]) >> (NBit-1)), y0 + ((length * COSPHASE256_DATA[angle]) >> (NBit-1)),color);
  }
  

};





#endif
