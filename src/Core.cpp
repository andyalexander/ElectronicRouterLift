#include <Arduino.h>
#include "Core.h"



Core :: Core( StepperDrive *stepperDrive )
{
    this->stepperDrive = stepperDrive;

    heightDelta = 0.0;
}


void Core :: powerToggle(void)
{
    this->stepperDrive->powerToggle();
}

bool Core :: getPowerState(void)
{
    return this->stepperDrive->getPowerState();
}

bool Core :: getLimitState(void)
{
    return false;
}






