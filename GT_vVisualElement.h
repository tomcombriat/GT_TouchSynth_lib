#ifndef _GT_VVISUALELEMENT_
#define _GT_VVISUALELEMENT_



/**
   A visual element (on screen) from which inherit all other visual elements
*/


// The different types available
#define vPotentiometer 1
#define vButton 2

#include <Adafruit_GFX.h>

class GT_vVisualElement
{
public:
  GT_vVisualElement(Adafruit_ILI9341* _screen, uint8_t type,unsigned long response_time=50):  screen{_screen}, type{type}, response_time{response_time} {}


  /**
     Select (or unselect) the element, normally after a click
  */
  void select(bool _select=true) {
    if (!selected && _select) last_select_time = millis();
    selected=_select;
  }

  /**
     Pres-select (or unpre-select) the element, normally while roaming between all the elements
  */
  void preselect(bool _preselect=true) {
    if (!preselected && _preselect) last_preselect_time = millis();
    preselected=_preselect;
  }

  /**
     Return the select state of the element
  */
  bool isSelected() {return selected;}


  
  /**
     Return the preselect state of the element
  */
  bool isPreselected() {return preselected;}

  /**
     Returns the type of the visual element:
     1: vPot
     2: vButton
  */
  const uint8_t getType()  {return type;}
  

  

protected:
  Adafruit_ILI9341 * const screen;
  unsigned long last_update, last_select_time, last_preselect_time;
  const unsigned long response_time;

private:
  bool selected=false, preselected=false;
  const uint8_t type;
  


};





#endif
