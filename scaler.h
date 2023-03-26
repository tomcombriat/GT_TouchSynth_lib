#ifndef SCALER_H_
#define SCALER_H_

template<typename T1, typename T2>
  inline T2 scale(T1 inputValue, byte inputNBit = sizeof(T1)>>3, byte outputNbit=sizeof(T2)>>3)
{
  /*
if (inputNBit > NBit)  value = _value >> (inputNBit - NBit);
    else if (inputNBit < NBit) value = _value >> ( NBit - inputNBit);
    else value = _value;*/

 T1 ret;
 if (inputNBit > outputNbit) ret = inputValue >> (inputNBit - inputNBit);
 else if (inputNBit < outputNbit) ret = inputValue << (outputNbit - inputNBit);
 else ret = inputValue;
 return ret;
}


#endif
