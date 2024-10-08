#ifndef _GT_ACTION_
#define _GT_ACTION_

#include<mozzi_rand.h>
#include"GT_Parameter.h"

/**
This file defines a set of *actions* that can be triggered.
They rely (for now) on a set of global variables being defined in the 
main code, for instance
 - N_PARAM
 - allParams
 - N_INPUTS
 - allInputs
 - ...
*/

inline void disconnectAllParam()
{
  for (byte i=0;i<N_PARAM;i++)
    {
      if (allParams[i]) allParams[i]->disconnectInput(); // check if not nullptr
   }

}

inline void randomizeAllParam()
{
  for (byte i=0;i<N_PARAM;i++)
    {
      if (allParams[i]) allParams[i]->setValue(rand(65536),16);

	}

}



#endif
