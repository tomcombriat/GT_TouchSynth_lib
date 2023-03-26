#ifndef PARAMETER_H_
#define PARAMETER_H_

#include "scaler.h"

/**********
 * Parameter that interacts with Mozzi, should be fast
   Should in the end be able to handle LUT for modifying the inputs on the fly.
 */







class Parameter
{
 public:
  /** Construtor
   */
  Parameter(byte _NBit=16){NBit = _NBit; value = 0;}
  /** Return the value of the parameter
@return int value
  */
  int getValue(){return value;}

    /** Return the value of the parameter in another type (with scaling to fit the new container)
@return T value
  */
  template<typename T>
    T getValue(byte outputNBits = sizeof(T)<<3)  {return scale<int,T>(value, NBit, outputNBits);}

  
  /** Set the value of the parameter
@param int _value the new value of the parameter
  */  
  void setValue(int _value){value = _value;}
    
  template<typename T2>
    void setValue(T2 _value, byte inputNBit = sizeof(T2)<<3) {value = scale<T2,int>(_value, inputNBit, NBit);}
  
  byte getNBit(){return NBit;}

 private:
  byte NBit;
  int value;
};


#endif
