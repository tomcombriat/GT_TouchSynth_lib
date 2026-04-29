#ifndef GT_PARAMETER_H_
#define GT_PARAMETER_H_

#include "scaler.h"

// Forward declaration
class GT_PhysicalInput;

template<typename T> constexpr T GT_shiftR(T x, int8_t bits) {return (bits > 0 ? (x >> (bits)) : bits < 0 ? (x << (-bits)) : x);}


class GT_Parameter
{
public:
  /** Constructor
   */
  GT_Parameter(const String name, const bool signedd, const int8_t NBits, GT_PhysicalInput* const* allInputs, const int8_t NInputs);

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
     Set the value of the parameter, in RAW
     @param _value: the value to input
     @param NBits_input the number of bits of the input
  */
  inline void setValueRAW(int32_t _value) // assumes unsigned
  {	
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

  /** Disconnect the input to the param (initialize the idxs to 0)
   */
  void disconnectInput();

  /**
     Return the name of the parameter
  */
  inline const String getName() const {return name;}

  /** 
      Return the number of bits of the parameter
  */
  inline constexpr int8_t getNBits() const {return NBits;}
  
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
     Set the physical input of the parameter, directly and immediately
  */
  void setInput(GT_PhysicalInput * _input,bool idx_known=false);

  /**
     Set the physical input of the parameter as the Nth of the complete list of parameters
  */
  void setInput(int N);

  /**
     Get the physical input of the parameter
  */
  inline GT_PhysicalInput * getInput() {return physical_input;}

  /**
     Get the physical input of the parameter
  */
  inline GT_PhysicalInput * getProspectiveInput() {return prospective_input;}

  /**
     Increment the physical input
     @todo TO BE REMOVED?
  */
  void incrementInput(int8_t inc);

  /**
     Increment the prospective input. 
     The prospective input will be selected to replace the current input
     after a timout
  */
  void incrementProspectiveInput(int8_t inc=1);

  inline unsigned long getLastProspectiveChangeTime() const
  {
    return last_prospective_change;
  }

  /**
     Update if the prospective input is to be selected
  */
  inline void update();
  
  /**
     Return the midi channel the parameter is watching
  */
  inline int8_t getMidiChannel() {return midi_channel;}

  /**
     Return the midi control1 (MSB) the parameter is watching
  */
  inline int8_t getMidiControl1() const {return midi_control1;}

  /**
     Return the midi control2 (LSB) the parameter is watching
  */
  inline int8_t getMidiControl2() const {return midi_control2;}

  /**
     Set the midi channel the parameter is watching
  */
  inline void setMidiChannel(byte _midi_channel) 
  {
    midi_channel=_midi_channel;
    if (midi_channel >15) midi_channel=15;
    if (midi_channel< -1) midi_channel=-1;
  }

  /**
     Set the midi control1 (MSB) the parameter is watching
  */
  inline void setMidiControl1(int8_t _control) 
  {
    midi_control1=_control;
    if (_control < -1)
      {
	midi_control1 = -1;
	midi_control2 = -1; // disabling both    
      }
  }

  /**
     Set the midi control2 (LSB) the parameter is watching
  */
  inline void setMidiControl2(int8_t _control) 
  {
    midi_control2=_control;
    if (_control <-1) midi_control2 = -1;
  }

  /**
     Notify of a MIDI input control change
  */
  void notifyMIDI(byte _channel, byte _control, byte _value);
  
   

  
  
private:
  const String name;
  const bool signedd;
  const int8_t NBits, NInputs;
  int8_t current_input_idx,prospective_input_idx;
  int32_t value;
  const int32_t max_value;
  const int32_t min_value;
  const int32_t bias;
  int8_t midi_channel, midi_control1, midi_control2; // -1 is non active
  GT_PhysicalInput * physical_input, *prospective_input;
  GT_PhysicalInput* const* allInputs;
  unsigned long last_prospective_change, prospective_timeout;
  
};




#endif
