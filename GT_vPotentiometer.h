#ifndef vPOTENTIOMETER_H_
#define vPOTENTIOMETER_H_

/****
 * Visual representation of a potentiometer.
 * TODO:
 - make indication of select/proposed input in the center:
 - pre-selected: blink in the center
 - selected: solid continuous color
 - from pre-selected: select/unselect with one click
 - exit menu (unselect) with one long click (for the rotary…)
 - go to menu on double click
 - put an adress to the menu in there (updated via this update)
 - maybe make size constant to have a potential canvas of just the size to minimize flicker?
*/

/*
  #define INNER_DISC 220
  #define INDICATOR_LENGTH 210*/
#define TEXT_BASE_WIDTH 6 // 5+1 spacing
#define TEXT_BASE_HEIGHT 7


#include <Adafruit_GFX.h>
#include "GT_Parameter.h"
#include "GT_vVisualElement.h"

/** A virtual class for different kinds of potentiometers
 */
template<typename T>
class GT_vPotentiometer: public GT_vVisualElement
{
public:
  GT_vPotentiometer(Adafruit_ILI9341* _screen, unsigned long response_time=50): GT_vVisualElement(_screen, 1, response_time)
  { NBit = sizeof(T)<<3;}

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

  /** 
      Preselect the visual pot
  */
  void preselect(bool _preselected=true)
  {
    preselected = _preselected;
  }

  /** 
      Select the visual pot
  */
  void select(bool _selected=true)
  {
    selected = _selected;
  }

  
  /** 
Return the preselction state of the vPot
  */
  bool isPreselected() const
  {
    return preselected;
      }

  /**
Return the selection state of the element
  */
    bool isSelected() const
  {
    return selected;
      }
  
  /** Set the size of the text of the potentiometer
      @param _text_size the new text size
  */
  void setTextSize(uint8_t _text_size) {text_size = _text_size;}
  
  void setText(String _text) {text = _text;}

  virtual void drawAll(){};
  virtual void update(){};

protected:
  // Adafruit_ILI9341 * screen;
  int16_t pos_X, pos_Y, size;
  uint16_t value, text_size;
  uint16_t color, background_color,default_color = 0xFFFF;
  byte NBit;
  bool visible;
  String text, long_text;
  unsigned long last_update, last_hit_time=0,  prospective_blink_time = 200, last_blink_time=0;
  //const unsigned long response_time;
  GT_Parameter * parameter=NULL;
  bool preselected = false, selected = false;
};





/** A classic visual representation of a potentiometer
 */
class GT_vClassicPot: public GT_vPotentiometer<uint8_t>
{
public:
  /** Constructor
      @param _screen is the pointer toward a screen
  */
  GT_vClassicPot(Adafruit_ILI9341* _screen, unsigned long response_time=50);
  
  /** Set the size of the visual potentiometer
      @param _size the new size
  */
  void setSize(int16_t _size);

  void setText(String _text);

  /** Update the display of the visual potentiometer if needed
      @note Timing: around 5-10ms
      @todo Change to get the *actual* value of the parameter, not the raw one. (on display?)
  */
  void update();

  void drawAll();

private:
  /// Members of this implementation
  int16_t old_pos_X, old_pos_Y, old_size;
  uint16_t old_color, in_color, old_in_color;
  uint8_t old_value;
  uint8_t max_string_length;
  bool refresh_text;
  GT_Parameter * old_parameter;
  
  

  static const int16_t INNER_DISC=220, INDICATOR_LENGTH=210, INDICATOR_WIDTH=20;


  //// Custom functions
  void drawLineAngle(int16_t x0,int16_t  y0,uint8_t value,int16_t length,uint16_t color);

  void drawFatLineAngle(int16_t x0,int16_t  y0,uint8_t value,int16_t length,uint16_t color);

  void eraseContour();

  void drawContour();

  void drawInContour();

  void eraseText();

  void drawText();

  void eraseValue();

  void drawValue();
};




#endif
