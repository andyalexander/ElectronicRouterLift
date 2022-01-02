#include "StepperDrive.h"

StepperDrive :: StepperDrive(void)
{
    this->power = false;
    resetPosition();
}

void StepperDrive :: resetPosition(void)
{
    // Set up global state variables
    this->stepsToMove = 0;

    // State machine starts at state zero
    this->state = 0;
}

int32_t StepperDrive :: getStepsFromHeight(double height)
{
    double revolutions = height / (MM_PER_INCH / LEADSCREW_TPI) ;
    int32_t steps = (int32_t)STEPPER_RESOLUTION*STEPPER_MICROSTEPS*revolutions;
    return steps;
}


void StepperDrive :: initHardware(void)
{
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIRECTION_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);
    pinMode(ALARM_PIN, INPUT);
    
    digitalWrite(STEP_PIN, LOW);
    digitalWrite(DIRECTION_PIN, LOW);

    digitalWrite(ENABLE_PIN, LOW);  // start off
}

void StepperDrive :: powerToggle(void)
{
    this->power = ! this->power;
    digitalWrite(ENABLE_PIN, this->power);
}

bool StepperDrive :: getPowerState()
{
    return this->power;
}






