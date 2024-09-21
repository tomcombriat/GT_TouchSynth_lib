#ifndef GT_TOUCHSCREEN_H_
#define GT_TOUCHSCREEN_H_


#include <XPT2046_Touchscreen.h>

class GT_Touchscreen
{
public:
  GT_Touchscreen(XPT2046_Touchscreen * const ts, const uint16_t size_x, const uint16_t size_y, const unsigned long response_time):ts{ts}, response_time{response_time}, size_x{size_x}, size_y{size_y} {;}


void update()
  {
    has_been_pressed = false;
    has_been_released = false;
    has_been_released_after_long_press=false;
    
    if (millis() - last_update_time > response_time)
      {
	last_update_time = millis();

	bool currently_pressed = ts->touched();

	if (currently_pressed && !is_pressed)
	  {
	    has_been_pressed=true;
	    last_press_time = millis();
	  }
	if (!currently_pressed && is_pressed)
	  {
	    //has_been_released=true;
	    if (millis()-last_press_time > long_press_timout) has_been_released_after_long_press=true;
	    else has_been_released=true;
	  }

	is_pressed = currently_pressed;
	if (is_pressed)
	  {
	    uint16_t xnew, ynew;
	    uint8_t znew;
	    ts->readData(&xnew, &ynew, &z);
	    x = map(xnew, MINX, MAXX, 0, size_x);
	    y = map(ynew, MINY, MAXY, 0, size_y);
	  }
      }
  }

  void calib(uint16_t _MINX, uint16_t _MINY, uint16_t _MAXX, uint16_t _MAXY)
  {
    MINX = _MINX;
    MINY = _MINY;
    MAXX = _MAXX;
    MAXY = _MAXY;
  }

  bool isPressed() {return is_pressed;}

  bool hasBeenReleased() {return has_been_released;}

    bool hasBeenReleasedAfterLongPress() {return has_been_released_after_long_press;}

  bool hasBeenPressed() {return has_been_pressed;}

void data(int16_t * _x, int16_t * _y)
  {
    *_x = x;
    *_y = y;
  }
  
  void data(int16_t * _x, int16_t * _y, uint8_t * _z)
  {
    data(_x,_y);
    *_z = z;
  }


  
private:
  XPT2046_Touchscreen * const ts;
  const unsigned long response_time;
  unsigned long last_update_time, last_press_time, long_press_timout=1000;
  int16_t x,y;
  uint8_t z;
  uint16_t MINX,MINY,MAXX,MAXY; // Calibration information
  const uint16_t size_x, size_y;

  bool is_pressed=false, has_been_pressed=false,has_been_released=false, has_been_released_after_long_press=false;
  

};


#endif
