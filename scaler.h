#ifndef SCALER_H_
#define SCALER_H_

template<typename T1, typename T2>
  inline T2 scale(T1 inputValue, byte inputNBit = sizeof(T1)<<3, byte outputNbit=sizeof(T2)<<3)
{
 T2 ret;
 if (inputNBit > outputNbit) ret = inputValue >> (inputNBit - outputNbit);
 else if (inputNBit < outputNbit) ret = inputValue << (outputNbit - inputNBit);
 else ret = inputValue;
 return ret;
}


#endif
