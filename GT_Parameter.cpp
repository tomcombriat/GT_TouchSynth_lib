#include "GT_Parameter.h"
#include "GT_Input.h"  // weird organization for circular inclusion

// GT_Parameter implementation

GT_Parameter::GT_Parameter(const String name, const bool signedd, const int8_t NBits, GT_PhysicalInput* const* allInputs, const int8_t NInputs):
    name{name},Signed{signedd},NBits{NBits}, allInputs{allInputs}, NInputs{NInputs},
    current_input_idx(0),prospective_input_idx(0),
    max_value((signedd ? 1<<(NBits-1) : 1<<NBits)-1),
    min_value(signedd ? -1<<(NBits-1):0),
    bias(signedd ? min_value : 0),
    midi_channel(-1), midi_control1(-1), midi_control2(-1),
    physical_input(nullptr), prospective_input(nullptr),
    last_prospective_change(0), prospective_timeout(2000)
{
  //setInput(allInputs[0]);
  setInput(0);
  incrementProspectiveInput(0);
}

void GT_Parameter::disconnectInput()
{
  if (physical_input) physical_input->removeTarget(&*this);
  prospective_input = nullptr;
  prospective_input_idx = 0;
  physical_input = nullptr;
  current_input_idx = 0;
}

void GT_Parameter::setInput(GT_PhysicalInput * _input, bool idx_known) 
{
  if (physical_input != nullptr) physical_input->removeTarget(&*this); 
  physical_input = _input;
  if (_input!=nullptr) _input->setTarget(&*this);
  if (!idx_known)
    {
      for (int8_t i=0;i<NInputs;i++)
	{
	  if (allInputs[i]==_input)
	    {
	      current_input_idx = i;
	      break;
	    }
	}
    }
  prospective_input = physical_input;
  prospective_input_idx = current_input_idx;
}

void GT_Parameter::setInput(int N)
{
  if (N<0) N=0;
  if (N>NInputs) N=NInputs;
  setInput(allInputs[N],true);
  current_input_idx = N;
  prospective_input = allInputs[N];
  prospective_input_idx = N;
}

void GT_Parameter::incrementProspectiveInput(int8_t inc)
{
  int8_t new_prospective_input = prospective_input_idx+inc;
  if (new_prospective_input > NInputs-1) new_prospective_input = 0;
  else if (new_prospective_input < 0) new_prospective_input = NInputs-1;
  prospective_input = allInputs[new_prospective_input];
  prospective_input_idx = new_prospective_input;
  last_prospective_change = millis();
}

void GT_Parameter::incrementInput(int8_t inc)
{
  int8_t new_current_input_idx = current_input_idx+inc;
  if (new_current_input_idx > NInputs-1) new_current_input_idx = 0;
  else if (new_current_input_idx < 0) new_current_input_idx = NInputs-1;
  setInput(allInputs[new_current_input_idx]);
  current_input_idx = new_current_input_idx;
}

void GT_Parameter::update()
{
  if (millis() - last_prospective_change > prospective_timeout) setInput(prospective_input_idx);
}

void GT_Parameter::notifyMIDI(byte _channel, byte _control, byte _value)
{
  if (physical_input != nullptr) // physical input takes precedence
    {
      if (_channel == midi_channel)
	{
	  if (_control == midi_control1) // MSB
	    {
	      if (midi_control2 == -1) setValue(_value, 7); // no LSB
	    
	      else // LSB present
		{
		  /* This is to avoid weird steppin with HQ Midi:
		     from below, we put the LSB to 0 (LSB might come higher, but that makes a monotonic transition) whereas coming from above we do the opposite
		    */
		  int32_t scaled_value = GT_shiftR((int32_t)_value, 7 - NBits);
		  if (Signed) value += 1<<(NBits-1); 
		  int32_t masked_old_value = GT_shiftR(127, 7 - NBits) & value;  // keep only the MSB
		  if (scaled_value > masked_old_value)  value = scaled_value;  
		  else if (scaled_value < masked_old_value) value = scaled_value + (GT_shiftR((int32_t)1, 7 - NBits)-1); // LSB to max value (next one is probably lower), MSB to real value.
		  else value = scaled_value + ((GT_shiftR(1,7-NBits)-1) & value); // all this to take into account double send of the same midi signal
		  // ((GT_shiftR(1,7-NBits)-1) & value) is keeping only the LSB, everything else at 0

		  /* Simplified version (for non doublon)
		     if (scaled_value > masked_old_value)  value = scaled_value;
		     else value = scaled_value + (GT_shiftR((int32_t)1, 7 - NBits)-1);
		    */
		  if (Signed) value -= 1<<(NBits-1);	    
		}  // else
	    
	    } // 	if (_control == midi_control1)
	  else if (_control == midi_control2)
	    {
	      int32_t scaled_value = GT_shiftR((int32_t)_value, 14 - NBits);
	      if (Signed) value += 1<<(NBits-1);
	      value = (GT_shiftR(127, 7 - NBits) & value) + scaled_value;
	      //        ONLY THE MSB
	      if (Signed) value -= 1<<(NBits-1);
	    }  // else if (_control == midi_control2)
	} // if (_channel == midi_channel)

    }
}
