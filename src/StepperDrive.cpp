#include "StepperDrive.h"

StepperDrive :: StepperDrive(void)
{
    this->power = false;                             // driver is enable low
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

double StepperDrive :: getHeightFromSteps(int32_t steps)
{
    double revolutions = (double)steps / (double)(STEPPER_RESOLUTION*STEPPER_MICROSTEPS);
    double height = (MM_PER_INCH / LEADSCREW_TPI) * revolutions;
    return height;

}


void StepperDrive :: initHardware(void)
{
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIRECTION_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);
    // pinMode(ALARM_PIN, INPUT);
    
    digitalWrite(STEP_PIN, INVERT_STEP_PIN^LOW);                // ^ is XOR
    digitalWrite(DIRECTION_PIN, INVERT_DIRECTION_PIN^LOW);
    powerSet(false);                                            // start with power off 

    // digitalWrite(LED_BUILTIN, HIGH);

    Serial.println("Stepper initialised...");
}

void StepperDrive :: powerToggle(void)
{
    this->power = ! this->power;
    digitalWrite(ENABLE_PIN, INVERT_ENABLE_PIN ^ (this->power));
}

void StepperDrive :: powerSet(bool state)
{
    this->power = state;
    digitalWrite(ENABLE_PIN, INVERT_ENABLE_PIN ^ state);
}

bool StepperDrive :: getPowerState()
{
    return !this->power;
}






