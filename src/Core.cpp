#include <Arduino.h>
#include "Core.h"
#include "Configuration.h"



Core :: Core( StepperDrive *stepperDrive )
{
    this->stepperDrive = stepperDrive;

    heightDelta = 0.0;
    isMove = false;
    //limitPanicSteps = 0;
    isHoming = false;
    //wasAtLimit = false;
    limitClearReads = 0;
}


void Core :: powerToggle(void)
{
    this->stepperDrive->powerToggle();
    this->cancelHome();
}

bool Core :: getPowerState(void)
{
    return this->stepperDrive->getPowerState();
}

bool Core :: getTopLimitState(void)
{
    bool is_limit = false;

    if (USE_SENSOR_TOP)
    {
        int sensor_reading = analogRead(SENSOR_TOP);
        //   Serial.println(sensor_reading);

        if (sensor_reading < SENSOR_TOP_THRESHOLD) {
            //this->wasAtLimit = true;
            this->limitClearReads = SENSOR_TOP_CLEAR_READS;     // reset the counter
            is_limit = true;
        }
        else {
            // check if we have been there before and not cleared yet
            this->limitClearReads = this->limitClearReads - 1;
            this->limitClearReads = max(0, this->limitClearReads);

            // Serial.println(this->limitClearReads);

            if (this->limitClearReads > 0){
                is_limit = true;
            }
        }
    }
    return is_limit;
}

bool Core :: getBitLimitState(void){
    bool is_limit = false;

    bool sensor_reading = !digitalRead(SENSOR_BIT);

      Serial.println(sensor_reading);

    if (sensor_reading) {
        is_limit = true;
    }
    return is_limit;
}

bool Core :: getLimitState(void)
{
    bool topLimit = getTopLimitState();
    bool bitLimit = getBitLimitState();

    return (topLimit || bitLimit);
}






