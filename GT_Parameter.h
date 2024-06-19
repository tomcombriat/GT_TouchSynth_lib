#ifndef GT_PARAMETER_H_
#define GT_PARAMETER_H_

#include "scaler.h"
//#include "GT_Input.h"

// Forward declaration
class GT_PhysicalInput;

template<typename T> constexpr T GT_shiftR(T x, int8_t bits) {return (bits > 0 ? (x >> (bits)) : bits < 0 ? (x << (-bits)) : x);}


class GT_Parameter
{
public:
  /** Constructor
   */
  GT_Parameter(const String name, const bool signedd, const int8_t NBits):
    name{name},signedd{signedd},NBits{NBits}
  {
  }


  /**
     Return the value of the parameter
  */
  inline int32_t getValue() const {return value;}

  /**
     Set the value of the parameter
     @param _value: the value to input
     @param NBits_input the number of bits of the input
  */
  inline void setValue(int32_t _value, int8_t NBits_input) // assumes unsigned
  {
    _value = GT_shiftR(_value, NBits_input-NBits);
    if (signedd) _value -= 1<<(NBits-1);  // might be worth storing this value once for all?	
    value = _value;
  }

  /**
     Increment (or decrement) the parameter
     @param _value: the value to add to the Parameter
  */
  inline void incrementValue(int32_t _value)
  {
    value += _value;
    if (value > max_value) value = max_value;
    else if (value < min_value) value = min_value;
  }


  /**
     Return the name of the parameter
  */
  inline const String getName() const {return name;}

  /** 
      Return the number of bits of the parameter
  */
  inline const int8_t getNBits() const {return NBits;}
  
  /**
     Return the bias of the parameter (0 if unsigned)
  */
  inline const int32_t getBias() const {return bias;}

  /**
     Return the max value of the parameter
  */
  inline const int32_t getMax() const {return max_value;}
  
  /**
     Return the min value of the parameter
  */
  inline const int32_t getMin() const {return min_value;}

  /**
     Set the physical input of the parameter
  */
  void setInput(GT_PhysicalInput * _input);
  

  /**
     Return the midi channel the parameter is watching
  */
  inline byte getMidiChannel() {return midi_channel;}

  /**
     Return the midi control1 (MSB) the parameter is watching
  */
  inline byte getMidiControl1() const {return midi_control1;}

  /**
     Return the midi control2 (LSB) the parameter is watching
  */
  inline byte getMidiControl2() const {return midi_control2;}

  /**
     Set the midi channel the parameter is watching
  */
  inline void setMidiChannel(byte _midi_channel) {midi_channel=_midi_channel;}

  /**
     Set the midi control1 (MSB) the parameter is watching
  */
  inline void setMidiControl1(byte _control) {
    midi_control1=_control;
    if (_control > 127)
      {
	midi_control1 = 255;
	midi_control2 = 255; // disabling both    
      }
  }

  /**
     Set the midi control2 (LSB) the parameter is watching
  */
  inline void setMidiControl2(byte _control) {
    midi_control2=_control;
    if (_control > 127) midi_control2 = 255;
  }

  /**
     Notify of a MIDI input control change
  */
  inline void notifyMIDI(byte _channel, byte _control, byte _value)
  {
    if (_channel == midi_channel)
      {
	if (_control == midi_control1) // MSB
	  {
	    if (midi_control2 == 255) setValue(_value, 7); // no LSB
	    
	    else // LSB present
	      {
		/* This is to avoid weird steppin with HQ Midi:
		   from below, we put the LSB to 0 (LSB might come higher, but that makes a monotonic transition) whereas coming from above we do the opposite
		*/
		int32_t scaled_value = GT_shiftR((int32_t)_value, 7 - NBits);
		if (signedd) value += 1<<(NBits-1); 
		int32_t masked_old_value = GT_shiftR(127, 7 - NBits) & value;  // keep only the MSB
		if (scaled_value > masked_old_value)  value = scaled_value;  
		else if (scaled_value < masked_old_value) value = scaled_value + (GT_shiftR((int32_t)1, 7 - NBits)-1); // LSB to max value (next one is probably lower), MSB to real value.
		else value = scaled_value + ((GT_shiftR(1,7-NBits)-1) & value); // all this to take into account double send of the same midi signal
		// ((GT_shiftR(1,7-NBits)-1) & value) is keeping only the LSB, everything else at 0

		/* Simplified version (for non doublon)
		   if (scaled_value > masked_old_value)  value = scaled_value;
		   else value = scaled_value + (GT_shiftR((int32_t)1, 7 - NBits)-1);
		*/
		if (signedd) value -= 1<<(NBits-1);	    
	      }  // else
	    
	  } // 	if (_control == midi_control1)
	else if (_control == midi_control2)
	  {
	    int32_t scaled_value = GT_shiftR((int32_t)_value, 14 - NBits);
	    if (signedd) value += 1<<(NBits-1);
	    value = (GT_shiftR(127, 7 - NBits) & value) + scaled_value;
	    //        ONLY THE MSB
	    if (signedd) value -= 1<<(NBits-1);
	  }  // else if (_control == midi_control2)
      } // if (_channel == midi_channel)


  }
  
   

  
  
private:
  const String name;
  const bool signedd;
  const int8_t NBits;
  int32_t value;
  const int32_t max_value=(signedd ? 1<<(NBits-1) : 1<<NBits)-1;
  const int32_t min_value=(signedd ? -1<<(NBits-1):0);
  const int32_t bias = (signedd ? min_value : 0);
  byte midi_channel, midi_control1=255, midi_control2=255; // 255 is non active
  GT_PhysicalInput * physical_input = NULL;
  
};


#include "GT_Input.h"

void GT_Parameter::setInput(GT_PhysicalInput * _input) {
  physical_input = _input;
  _input->setTarget(&*this);
}




#endif
