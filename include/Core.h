#ifndef __CORE_H
#define __CORE_H

#include "StepperDrive.h"
#include "ControlPanel.h"

class Core
{
private:
    StepperDrive *stepperDrive;

    // float feed;
    // float previousFeed;

    //double heightCurrent;           // actual height of the router
    //double heightTarget;            // target height of the router
    volatile double heightDelta;      // how much to move by
    volatile bool isMove;

    int16_t limitPanicSteps;            // number of steps taken under the limit condition

    int32_t feedRatio(uint32_t count);

public:
    Core( StepperDrive *stepperDrive );

    // void setFeed(const FEED_THREAD *feed);
    // void setReverse(bool reverse);
    // uint16_t getRPM(void);
    double getHeight(void);
    
    bool isAlarm();
    bool isReady();

    void myISR( void );

    void powerToggle(void);
    bool getPowerState(void);
    bool getLimitState(void); // are we at a limit
    int32_t getStepsFromHeight(double height);

    void setHeightDelta(double height);
    int32_t getStepsRemaining(void);
};

inline void Core :: setHeightDelta(double height)
{
    heightDelta = height;
}

inline bool Core :: isAlarm()
{
    return this->stepperDrive->isAlarm();
}

inline bool Core :: isReady()
{
    return !isMove && this->getPowerState();
}

inline int32_t Core :: getStepsFromHeight(double height)
{
    return stepperDrive->getStepsFromHeight(height);
}

inline int32_t Core :: getStepsRemaining()
{
    return stepperDrive->getStepsRemaining();
}

inline void Core :: myISR( void )
{
    if (getLimitState())
    {
        digitalWrite(LED_BUILTIN, HIGH);
        this->stepperDrive->powerSet(true);         // stepper is enable low
        // Serial.println("Sensor tripped");
    }
    else{
        digitalWrite(LED_BUILTIN,LOW);
    }

    if( abs(heightDelta) > 0.01) {           // if we need to move...
        Serial.print("Need to move: ");
        Serial.print(heightDelta);
        // calculate the desired stepper position
        int32_t desiredSteps =  stepperDrive->getStepsFromHeight(heightDelta);

        stepperDrive->setStepsToMove(desiredSteps);
        Serial.print(" (");
        Serial.print(desiredSteps);
        Serial.println("steps)");

        // service the stepper drive state machine
        heightDelta = 0.0;
    }

    if (stepperDrive->getStepsRemaining() != 0)
    {
        isMove = true;
        stepperDrive->myISR();
    } else {
        isMove = false;
    }
}



#endif // __CORE_H
