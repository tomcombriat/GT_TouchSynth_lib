#include "GT_Parameter.h"
#include "GT_Input.h"




void GT_Parameter::disconnectInput()
{
  if (physical_input) physical_input->removeTarget(&*this);
  prospective_input = nullptr;
  prospective_input_idx = 0;
  physical_input = nullptr;
  current_input_idx = 0;
}

void GT_Parameter::setInput(GT_PhysicalInput * _input, bool idx_known) {
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

void GT_Parameter::commitProspectiveInput()
{
  if (prospective_input_idx != current_input_idx) setInput(prospective_input_idx);
}

void GT_Parameter::update()
{
  if (millis() - last_prospective_change > prospective_timeout) setInput(prospective_input_idx);
}
