#include "GT_Input.h"
#include "GT_Menu.h"

void GT_Menu::start(){
  encoder->setTargetMenu(&*this);
  is_active=true;
}


void GT_Menu::exit(){
    encoder->setTargetMenu(nullptr);
    is_active=false;
  }
