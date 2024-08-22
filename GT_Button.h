#ifndef _GT_BUTTON_
#define _GT_BUTTON_

#include"vPotentiometer.h" // to recover the text sizes


/**
   A button, present on screen, that can be pressed to trigger an action
*/

#include <Adafruit_GFX.h>

/** 
    Base class for all buttons
*/

class GT_Button
{
public:
  GT_Button(Adafruit_ILI9341* _screen, unsigned long response_time=50): response_time{response_time} 
  {screen = _screen;}

    /** Set the position of the button
      @param X the X position
      @param Y the Y position
  */
  void setPosition(int16_t X, int16_t Y){pos_X = X; pos_Y = Y;}


    /** Set the color of the button
      @param _color the new color
  */
  void setColor(uint16_t _color) {color = _color;}

    /** Probe if a point (X,Y) is touching the object
      @param X the horinzontal position of the point
      @param Y the vertical position of the point
      @return boolean telling if the point is touching the object
  */
  virtual bool isInHitBox(int16_t X, int16_t Y) {return false;}

  virtual void update(){}

  void setAction(void (*_action)()) {action = _action;}

  void trigAction()
  {
    if (action) action();
  }


protected:
  Adafruit_ILI9341 * screen;
  int16_t pos_X, pos_Y;
  uint16_t color, old_color, background_color;
  unsigned long last_update;
  const unsigned long response_time;
  void (*action)()=nullptr;
};



class GT_BasicButton: public GT_Button
{
public:
  GT_BasicButton(Adafruit_ILI9341* _screen, unsigned long response_time=50): GT_Button(_screen, response_time){} /*response_time{response_time} 
														   {screen = _screen;}*/
  



  /** Set the position of the button
      @param X the X size
      @param Y the Y size
  */
  void setSize(int16_t X, int16_t Y){size_X = X; size_Y = Y;}

  
  /** Set the position and the size of the button
      @param X the X position
      @param Y the Y position
      @param width
      @param height
  */
  void setPositionAndSize(int16_t X, int16_t Y, int16_t width, int16_t height)
  {
    setPosition(X,Y);
    setSize(width, height);
  }


  /** Set the color of the button's text
      @param _color the new color
  */
  void setTextColor(uint16_t _color) {text_color = _color;}


  /** Set the text displayed by the button
      @param _text the new text of the Button
  */
  void setText(String _text) {text = _text;} 


  /** Probe if a point (X,Y) is touching the object
      @param X the horinzontal position of the point
      @param Y the vertical position of the point
      @return boolean telling if the point is touching the object
  */
  bool isInHitBox(int16_t X, int16_t Y)
  {
    if (X > pos_X && X < pos_X + size_X && Y > pos_Y && Y < pos_Y + size_Y) return true;
    return false;
  }

  void update()
  {
    if (millis() - last_update > response_time)
      {
	if (old_pos_X != pos_X || old_pos_Y != pos_Y || old_size_X != size_X || old_size_Y != size_Y)
	  {
	    eraseBox();
	    drawBox();
	    drawText();
	    old_pos_X = pos_X;
	    old_pos_Y = pos_Y;
	    old_size_X = size_X;
	    old_size_Y = size_Y;
	    old_text = text;
	    old_color = color;
	    old_text_color = text_color;
	  }
	else if (old_color != color)
	  {
	    drawBox();
	    drawText();
	    old_text = text;
	    old_color = color;
	    old_text_color = text_color;
	  }
	else if (old_text != text)
	  {
	    eraseText();
	    drawText();
	  }
	last_update = millis();
      }
  }



private:
  
  int16_t size_X, size_Y;
  int16_t old_pos_X, old_pos_Y, old_size_X, old_size_Y;
  uint16_t text_color;
  uint16_t  old_text_color;
  uint8_t text_size=1, old_text_size=0;
  String text, old_text;



  void drawBox() {screen->fillRect(pos_X, pos_Y, size_X, size_Y, color); }

  void eraseBox() {screen->fillRect(old_pos_X, old_pos_Y, old_size_X, old_size_Y, background_color); }

  void drawText() {
    screen->setCursor(pos_X + (size_X>>1) - ((text.length()*(TEXT_BASE_WIDTH>>1))), pos_Y + (size_Y>>1) - (TEXT_BASE_HEIGHT>>1));
    screen->setTextColor(text_color);
    screen->print(text);
  }

  void eraseText() {
    screen->setCursor(old_pos_X + (old_size_X>>1) - ((old_text.length()*(TEXT_BASE_WIDTH>>1))), old_pos_Y + (old_size_Y>>2) - (TEXT_BASE_HEIGHT>>1));
    screen->setTextColor(color);
    screen->print(old_text);
  }
};




#endif
