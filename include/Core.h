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
    return isMove;
}

inline int32_t Core :: getStepsFromHeight(double height)
{
    return stepperDrive->getStepsFromHeight(height);
}

inline void Core :: myISR( void )
{
    if( abs(heightDelta) > 0.01) {           // if we need to move...
        Serial.print("Need to move: ");
        Serial.print(heightDelta);
        // calculate the desired stepper position
        int32_t desiredSteps =  stepperDrive->getStepsFromHeight(heightDelta);

        stepperDrive->setStepsToMove(desiredSteps);
        Serial.print(" (");
        Serial.print(desiredSteps);
        Serial.println("steps)");


        // remember values for next time
        // previousSpindlePosition = spindlePosition;
        // previousFeedDirection = feedDirection;
        // previousFeed = feed;

        // service the stepper drive state machine
        heightDelta = 0.0;
    }

    if (stepperDrive->getStepsRemaining() != 0)
    {
        isMove = true;
        // Serial.println("step");
        stepperDrive->myISR();
    } else {
        isMove = false;
    }
}



#endif // __CORE_H
