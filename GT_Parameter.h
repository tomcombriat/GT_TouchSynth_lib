#ifndef GT_PARAMETER_H_
#define GT_PARAMETER_H_



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
  inline int32_t getValue() {return value;}

    /**
     Set the value of the parameter
@param _value: the value to input
@param NBits_input the number of bits of the input
  */
  inline void setValue(int32_t _value, int8_t NBits_input) // assumes unsigned
  {
    if (signedd)
      {
	 _value = GT_shiftR(_value, NBits_input-NBits);
	_value -= 1<<(NBits-1);	
      }
    else  _value = GT_shiftR(_value, NBits_input-NBits);
    value = _value;
  }


  /**
     Return the name of the parameter
  */
  inline const String getName() {return name;}

  /**
     Return the max value of the parameter
  */
  inline const int32_t getMax() {return max_value;}
  
  /**
     Return the min value of the parameter
  */
  inline const int32_t getMin() {return min_value;}

  /**
     Return the midi channel the parameter is watching
  */
  inline byte getMidiChannel() {return midi_channel;}

  /**
     Return the midi control1 (MSB) the parameter is watching
  */
  inline byte getMidiControl1() {return midi_control1;}

  /**
     Return the midi control2 (LSB) the parameter is watching
  */
  inline byte getMidiControl2() {return midi_control2;}

    /**
     Set the midi channel the parameter is watching
  */
  inline void setMidiChannel(byte _midi_channel) {midi_channel=_midi_channel;}

  /**
     Set the midi control1 (MSB) the parameter is watching
  */
  inline void setMidiControl1(byte _control) {midi_control1=_control;}

  /**
     Set the midi control2 (LSB) the parameter is watching
  */
  inline void setMidiControl2(byte _control) {midi_control2=_control;}


  
   

  
  
 private:
  const String name;
  const bool signedd;
  const int8_t NBits;
  int32_t value;
  const int32_t max_value=(signedd ? 1<<(NBits-1) : 1<<NBits)-1;
  const int32_t min_value=(signedd ? -1<<(NBits-1):0);
  byte midi_channel, midi_control1, midi_control2;
  
};









#endif
