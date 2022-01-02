#ifndef __STEPPERDRIVE_H
#define __STEPPERDRIVE_H

#include <Arduino.h>
#include "Configuration.h"


#define STEP_PIN        3
#define DIRECTION_PIN   4
#define ENABLE_PIN      5
#define ALARM_PIN       10

#define POSITION_TOL    1               // steps tolerance for 'exact' match
class StepperDrive
{
private:
    //
    // Current position of the motor, in steps
    //
    int32_t stepsToMove;

    //
    // current state-machine state
    // bit 0 - step signal
    // bit 1 - direction signal
    //
    uint16_t state;
    bool power;

public:
    StepperDrive();
    void initHardware(void);

    void resetPosition();

    void setStepsToMove(int32_t steps);
    int32_t getStepsFromHeight(double height);
    int32_t getStepsRemaining(void);

    bool isAlarm();

    void myISR(void);

    void powerToggle(void);
    bool getPowerState(void);
};


inline void StepperDrive :: setStepsToMove(int32_t steps)
{
    this->stepsToMove = steps;
}

inline int32_t StepperDrive :: getStepsRemaining(void)
{
    return this->stepsToMove;
}

inline bool StepperDrive :: isAlarm()
{
#ifdef USE_ALARM_PIN
    return digitalRead(ALARM_PIN);
    // return GPIO_GET_ALARM;
#else
    return false;
#endif
}


inline void StepperDrive :: myISR(void)
{
    switch( this->state ) {
    case 0:
        // Step = 0; Dir = 0
        if( this->stepsToMove < 0) {
            digitalWrite(STEP_PIN, LOW);
            this->state = 2;
        }
        else if( this->stepsToMove > 0 ) {
            digitalWrite(DIRECTION_PIN, HIGH);
            this->state = 1;
        }
        break;

    case 1:
        // Step = 0; Dir = 1
        if( this->stepsToMove > 0 ) {
            digitalWrite(STEP_PIN, LOW);
            this->state = 3;
        }
        else if( this->stepsToMove < 0) {
            digitalWrite(DIRECTION_PIN, LOW);
            this->state = 0;
        }
        break;

    case 2:
        // Step = 1; Dir = 0
        digitalWrite(STEP_PIN, HIGH);
        // GPIO_CLEAR_STEP;
        this->stepsToMove++;
        this->state = 0;

        break;

    case 3:
        // Step = 1; Dir = 1
        digitalWrite(STEP_PIN, HIGH);
        this->stepsToMove--;
        this->state = 1;

        break;
    }
}

#endif // __STEPPERDRIVE_H