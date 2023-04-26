#ifndef GT_INPUT_H_
#define GT_INPUT_H_
#include <mozzi_analog.h> // for mozziAnalogRead()

/*
 * Input refers to anything that could be inputted to the synthesizer, especially:
 * - analog inputs (potentiometers, expression pedal)
 * - MIDI inputs (normal and HQ)
 */

#include "scaler.h"

/*
 * This is a virtual class from which every type of input is derived
 */

class Input
{
  /** Constructor
   */
  Input(String _name)
  {
    name = _name;
  }
  
  /** Get the value of the Input
      @return the value of the Input.
  */
  inline int32_t getValue(){return value;}

  /** Get the name of the Input
      @return the name of the Input.
  */
  String getName(){return name;}

  /** Update the Input (not needed for every specification)
   */
  virtual void update(){}

protected:
  int32_t value;
  String name;
};


/*
 * This is a specialization for all ADC inputs: potentiometers, expression pedals
 */

template<byte NBit_ADC=10>
class AnalogInput: public Input
{
public:
  AnalogInput(String _name, int _pin): Input(_name){pin = _pin;}
  inline void update(){value = scale(mozziAnalogRead(pin), NBit_ADC, 16);}
private:
  int pin;
};





















#endif
