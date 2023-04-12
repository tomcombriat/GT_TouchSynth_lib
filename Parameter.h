#ifndef PARAMETER_H_
#define PARAMETER_H_

#include "scaler.h"

/**********
 * Parameter that interacts with Mozzi, should be fast
   Should in the end be able to handle LUT for modifying the inputs on the fly.
 */


/***
TOOD:
 - add update method which should take care of computations (LUT, scaling)
 - template on the return type (whole class templated ?)
 - internal calculations on int type?
 - how to handle unsigned and signed value neatly?
*/






class Parameter_virt
{
 public:
  /** Construtor
   */
  Parameter_virt(String _name){name = _name; NBit = 16; raw_value = 0;}


    /** Return the value of the parameter in another type (with scaling to fit the new container)
@return T value
    */
  /*  template<typename T2>
    T getValue(byte outputNBits = sizeof(T2)<<3)  {return scale<int,T2>(value, NBit, outputNBits);}
*/
  
   int32_t getRawValue(){return raw_value;}
  
  /** Set the value of the parameter
@param int _value the new value of the parameter
  */  
  void setRawValue(int32_t _value){raw_value = _value;}

  /** Set the value of the parameter coming from an other (positive) type
@param T _value the new *positive* value
@param inputNbit the actual bit depth of _value
  */
  
  template<typename T2>
  void setRawValue(T2 _value, byte inputNBit) {raw_value = scale<T2,int16_t>(_value, inputNBit, NBit);}
  
  byte getNBit(){return NBit;}

  String getName(){return name;}

  virtual void getValue(){}

  void update()
  {
    // apply lut, check for changes in bound input
  }

 protected:
  byte NBit;
  int32_t raw_value;   // contains the "working" value, positive 16bits (bigger container for easy scaling)
  String name;
};


#endif
