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

class GTInput
{
public:
  /** Constructor
   */
  GTInput(String _name)
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
template<byte NBit_ADC=12>
class AnalogInput: public GTInput
{
public:
  AnalogInput(String _name, int _pin): GTInput(_name){
    pin = _pin;
    pinMode(pin,INPUT);
  }
  
  void update(){
    int tamp_value = mozziAnalogRead(pin);
    if (invert) tamp_value=(1<<NBit_ADC)-1 - tamp_value;
    value = scale<int16_t, int32_t>(tamp_value, NBit_ADC, 16);
  }
  
  void setInvert(bool _invert) {invert = _invert;}
  
private:
  int pin;
  bool invert = false;
};



/**
 * Specialization for single precision MIDI inputs
 */
class MidiInput: public GTInput
{
public:
  /** Constructor
   */
  MidiInput(String _name, uint8_t _channel=0, uint8_t _control=0): GTInput(_name){channel = _channel; control = _control;}
  
  /** Get the MIDI channel of the parameter
      @return the MIDI channel
  */
  uint8_t getChannel(){return channel;}

  /** Get the MIDI control of the parameter
      @return the MIDI control
  */
  uint8_t getControl(){return control;}

  /** Set the MIDI channel of the parameter
      @param the new MIDI channel of the parameter
      @todo dealing with omni?
  */
  void setChannel(uint8_t _channel){channel = _channel;}

  /** Set the MIDI control of the parameter
      @param the new MIDI control of the parameter
  */
  void setControl(uint8_t _control){control = _control;}

  /** Set the MIDI value of the parameter, should not be used by the user (protected?)
      @param the new value of the parameter
  */
  void setValue(uint8_t _value){value = _value<<9;} // MIDI signals are 7 bits


  /** Send a MIDI message complete to the parameter, then it deals with it 
      @todo maybe use the framework of the MIDI library?
  */
  void setMIDIMessage(uint8_t _channel, uint8_t _control, uint8_t _value)
  {
    if (_control == control && _channel == channel) setValue(_value);
  }

  
private:
  uint8_t channel, control;
};




/**
 * Specialization for *double* precision MIDI inputs
 */
class MidiInputHQ: public GTInput
{
public:
  /** Constructor
   */
  MidiInputHQ(String _name, uint8_t _channel=0, uint8_t _controlMSB=0,uint8_t _controlLSB = 0): GTInput(_name){channel = _channel; controlMSB = _controlMSB; controlLSB = _controlLSB;}
  
  /** Get the MIDI channel of the parameter
      @return the MIDI channel
  */
  uint8_t getChannel(){return channel;}

  /** Get the MSB (Most Significant byte) MIDI control of the parameter
      @return the MSB MIDI control
  */
  uint8_t getControlMSB(){return controlMSB;}

  /** Get the LSB (Least Significant byte) MIDI control of the parameter
      @return the LSB MIDI control
  */
  uint8_t getControlLSB(){return controlLSB;}

  /** Set the MIDI channel of the parameter
      @param the new MIDI channel of the parameter
      @todo dealing with omni?
  */
  void setChannel(uint8_t _channel){channel = _channel;}

  /** Set the MSB MIDI control of the parameter
      @param the new MSB MIDI control of the parameter
  */
  void setControlMSB(uint8_t _controlMSB){controlMSB = _controlMSB;}

  /** Set the LSB MIDI control of the parameter
      @param the new LSB MIDI control of the parameter
  */
  void setControlLSB(uint8_t _controlLSB){controlLSB = _controlLSB;}

  /** Set the MSB MIDI value of the parameter, should not be used by the user (protected?)
      @param the new MSB value of the parameter
  */
  void setValueMSB(uint8_t _value){value = _value<<9;} // left align: 7bits for starters
  // TODO:there are some nice tricks here to be implemented for continuous signals (look 10k codes)

  /** Set the LSB MIDI value of the parameter, should not be used by the user (protected?)
      @param the new LSB value of the parameter
  */
  void setValueLSB(uint8_t _value){value += _value<<2;}

  /** Send a MIDI message complete to the parameter, then it deals with it 
      @todo maybe use the framework of the MIDI library?
  */
  void setMIDIMessage(uint8_t _channel, uint8_t _control, uint8_t _value)
  {
    if (_channel == channel)
      {	
	if (_control == controlMSB) setValueMSB(_value);
	else if (_control == controlLSB) setValueLSB(_value);
      }
  }
  
  
private:
  uint8_t channel, controlMSB, controlLSB;
};





















#endif
