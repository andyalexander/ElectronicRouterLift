#include <Arduino.h>
#include "Core.h"
#include "Configuration.h"



Core :: Core( StepperDrive *stepperDrive )
{
    this->stepperDrive = stepperDrive;

    heightDelta = 0.0;
    isMove = false;
    limitPanicSteps = 0;
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
    if (USE_SENSOR_TOP)
    {
        int sensor_reading = analogRead(SENSOR_TOP);
    //   float sensor_value = (float)sensor_reading*100/1024.0;
    //   Serial.println(sensor_reading);
        return sensor_reading < SENSOR_TOP_THRESHOLD;
    }
    else {return false;}
}






