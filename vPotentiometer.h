#ifndef vPOTENTIOMETER_H_
#define vPOTENTIOMETER_H_

/****
 * Visual representation of a potentiometer.
 * TODO:
 * - allow text size to be changed
 * - implement necessary getters
 * - setter for delay
 */

/*
  #define INNER_DISC 220
  #define INDICATOR_LENGTH 210*/
#define TEXT_BASE_SIZE 6
//#define MAX_STRING_LENGTH 10


#include <Oscil.h>   // Mozzi
#include <tables/sin256_int8.h>   // Mozzi
#include <tables/cosphase256_int8.h>   // Mozzi
#include <Adafruit_GFX.h>


/** A virtual class for different kinds of potentiometers
 */
template<typename T>
class vPotentiometer  
{
public:
  vPotentiometer(Adafruit_ILI9341* _screen){ screen = _screen; NBit = sizeof(T)<<3; update_delay = 50;}

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
  void setValue(T2 _value, byte inputNBit)
  {
    if (inputNBit > NBit)  value = _value >> (inputNBit - NBit);
    else if (inputNBit < NBit) value = _value >> ( NBit - inputNBit);
    else value = _value;
  }

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
    if (X > pos_X - (size>>1) && X <pos_X +(size>>1) && Y>pos_Y - (size>>1) && Y < pos_Y + (size>>1)) return true;
    else return false;
  }

  /** Set the time between each actual refresh of the object
      @param delay: the new refresh time
  */
  void setRefreshTime(unsigned long delay) {update_delay = delay};
  
  void setText(String _text) {text = _text;}
  virtual void update(){};

protected:
  Adafruit_ILI9341 * screen;
  int16_t pos_X, pos_Y, size;
  uint16_t value, text_size;
  uint16_t color, background_color;
  byte NBit;
  bool visible;
  String text;
  unsigned long update_delay, last_update;
};






/** A classic visual representation of a potentiometer
 */
class ClassicPot: public vPotentiometer<uint8_t>
{
public:
  /** Constructor
      @param _screen is the pointer toward a screen
  */
  ClassicPot(Adafruit_ILI9341* _screen): vPotentiometer(_screen){}
  
  /** Set the size of the visual potentiometer
      @param _size the new size
  */
  void setSize(int16_t _size) {
    size = _size;
    max_string_length = (size<<1)/TEXT_BASE_SIZE;
  }

  void setText(String _text) {
    refresh_text = true;
    if (_text.length() > max_string_length)
      {
	text = _text.substring(0,max_string_length-1) + ".";
      }
    else text = _text;
  }

  /** Set the size of the text of the potentiometer
      @param _text_size the new text size
  */
  void setTextSize(uint8_t _text_size) {text_size = _text_size;}
  

  /** Update the display of the visual potentiometer if needed
      @note Timing: around 5-10ms
  */
  void update()
  {
    if (millis() - last_update > update_delay)
      {	
	if (old_size != size)
	  {
	    screen->fillRect(old_pos_X - ((max_string_length*TEXT_BASE_SIZE)>>1), old_pos_Y + old_size + (TEXT_BASE_SIZE>>1), max_string_length*TEXT_BASE_SIZE, TEXT_BASE_SIZE+1,background_color); // delete text
	    refresh_text = true;
	    eraseAndDrawContour();  // delete and redraw the contour
	    drawFatLineAngle(pos_X,pos_Y,value,(size*INDICATOR_LENGTH)>>8,color);  // draw the indicator
	  }
	else if (old_pos_X != pos_X || old_pos_Y != pos_Y)
	  {
	    eraseAndDrawContour();
	    drawFatLineAngle(pos_X,pos_Y,value,(size*INDICATOR_LENGTH)>>8,color);
	  }
	else if (old_value != value)
	  {
	    drawFatLineAngle(pos_X,pos_Y,old_value,(size*INDICATOR_LENGTH)>>8,background_color);  // erase old indicator
	    drawFatLineAngle(pos_X,pos_Y,value,(size*INDICATOR_LENGTH)>>8,color);
	    old_value = value;	     
	  }
	else if (old_color != color)
	  {
	    screen->fillCircle(pos_X, pos_Y, size, color);
	    screen->fillCircle(pos_X, pos_Y, (size*INNER_DISC) >> 8, background_color);
	    drawFatLineAngle(pos_X,pos_Y,value,(size*INDICATOR_LENGTH)>>8,color);
	    screen->setCursor(pos_X - (text.length()*(TEXT_BASE_SIZE>>1)), pos_Y + size + (TEXT_BASE_SIZE>>1));
	    screen->setTextColor(color);
	    screen->print(text);
	  }

	if (refresh_text)
	  {
	    screen->fillRect(pos_X - ((max_string_length*TEXT_BASE_SIZE)>>1), pos_Y + size + (TEXT_BASE_SIZE>>1), max_string_length*TEXT_BASE_SIZE, TEXT_BASE_SIZE+1,background_color);	  
	    screen->setCursor(pos_X - (text.length()*(TEXT_BASE_SIZE>>1)), pos_Y + size + (TEXT_BASE_SIZE>>1));
	    screen->setTextColor(color);
	    screen->print(text);
	    refresh_text = false;
	  }

    
	old_pos_X = pos_X;
	old_pos_Y = pos_Y;
	old_value = value;
	old_size = size;
	old_color = color;

	last_update = millis();  
      }
  }



private:
  /// Members of this implementation
  int16_t old_pos_X, old_pos_Y, old_size;
  uint16_t old_color;  
  uint8_t old_value;
  uint8_t max_string_length;
  bool refresh_text;

  static const int16_t INNER_DISC=220, INDICATOR_LENGTH=210, INDICATOR_WIDTH=20;


  //// Custom functions
  void drawLineAngle(int16_t x0,int16_t  y0,uint8_t value,int16_t length,uint16_t color)
  {    
    uint8_t angle =((value *195) >> 8) +30;
    screen->drawLine(x0,y0,x0 -((length * SIN256_DATA[angle]) >> (NBit-1)), y0 + ((length * COSPHASE256_DATA[angle]) >> (NBit-1)),color);
  }

  void drawFatLineAngle(int16_t x0,int16_t  y0,uint8_t value,int16_t length,uint16_t color)
  {
    uint8_t angle =((value *195) >> 8) +30;
    int width = (size * INDICATOR_WIDTH)>>8;
    
    for (int i=-width/2; i<=width/2;i++) drawLineAngle( x0+((i*COSPHASE256_DATA[angle])>>(NBit-1)),  y0+((i*SIN256_DATA[angle])>>(NBit-1)), value, length, color);      
  }
  
  void eraseAndDrawContour()
  {
    screen->fillCircle(old_pos_X, old_pos_Y, old_size, background_color);
    screen->fillCircle(pos_X, pos_Y, size, color);
    screen->fillCircle(pos_X, pos_Y, (size*INNER_DISC) >> 8, background_color);
  }
  

};





#endif
