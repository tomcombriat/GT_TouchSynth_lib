#ifndef GT_INPUT_H_
#define GT_INPUT_H_
#include <mozzi_analog.h> // for mozziAnalogRead()

/*
 * Input refers to anything that could be inputted to the synthesizer, especially:
 * - analog inputs (potentiometers, expression pedal)
 * - MIDI inputs (normal and HQ)
 */


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
  
  /** Get the value of the input
   */
  int32_t getValue(){return value;}

  /** Get the name of the input
   */
  String getName(){return name;}

 protected:
  int32_t value;
  String name;
};

class ADCInput: public Input
{
 public:
  


 private:
};





















#endif
