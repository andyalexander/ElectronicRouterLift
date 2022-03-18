#ifndef __CORE_H
#define __CORE_H

#include "StepperDrive.h"
#include "ControlPanel.h"

class Core
{
private:
    // float feed;
    // float previousFeed;

    //double heightCurrent;           // actual height of the router
    //double heightTarget;            // target height of the router
    volatile double heightDelta;      // how much to move by
    volatile bool isMove;
    // volatile bool isGoingHome;

    //int16_t limitPanicSteps;            // number of steps taken under the limit condition
    int16_t limitClearReads;            // number of clear reads

    int32_t feedRatio(uint32_t count);

public:
    Core( StepperDrive *stepperDrive );
    StepperDrive *stepperDrive;

    // void setFeed(const FEED_THREAD *feed);
    // void setReverse(bool reverse);
    // uint16_t getRPM(void);
    //volatile bool wasAtLimit;         // were we at limit the last time

    double getHeight(void);
    
    bool isAlarm();
    bool isReady();
    bool isHoming; 
    // bool isHoming();

    void myISR( void );

    void goHome(void);
    void cancelHome(void);
    bool getLimitState(void); // are we at a limit

    void powerToggle(void);
    bool getPowerState(void);
    int32_t getStepsFromHeight(double height);
    double getHeightFromSteps(int32_t steps);

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

// inline void Core :: goHome()
// {
//     isHoming = true;
//     this->setHeightDelta(MAX_TRAVEL);
// }

inline void Core :: cancelHome()
{
    isHoming = false;
    heightDelta = 0;
    stepperDrive->setStepsToMove(0);
}

// inline bool Core :: isHoming()
// {
    // return isGoingHome;
// }

inline int32_t Core :: getStepsFromHeight(double height)
{
    return stepperDrive->getStepsFromHeight(height);
}

inline double Core :: getHeightFromSteps(int32_t steps){
    return stepperDrive->getHeightFromSteps(steps);
}

inline int32_t Core :: getStepsRemaining()
{
    return stepperDrive->getStepsRemaining();
}

inline void Core :: myISR( void )
{
    // first check the limit switches
    if (getLimitState())
    {
        digitalWrite(LED_BUILTIN, HIGH);
        // this->stepperDrive->powerSet(false);         // stop the motor if at limit

        if (this->isHoming) {
            this->cancelHome();
        }
        else if (stepperDrive->getStepsRemaining() > 0) {          // we are trying to go up, so not ok
            stepperDrive->setStepsToMove(0);
            heightDelta = 0.0;
        }
        // Serial.println("Sensor tripped");
    }
    else{
       // we are not at the limit, so can move
        digitalWrite(LED_BUILTIN,LOW);
    }

    // we are homing so introduce a step
    // if (this->isGoingHome){
        // heightDelta = 1;
    // }

    // if we need have a movement request from UI
    if( abs(heightDelta) > 0.01) {           
        // Serial.print("Need to move: ");
        // Serial.print(heightDelta);

        // calculate the desired stepper position
        int32_t desiredSteps =  stepperDrive->getStepsFromHeight(heightDelta);
        heightDelta = 0.0;                  // reset the height delta as have converted to steps

        stepperDrive->setStepsToMove(desiredSteps);
        // Serial.print(" (");
        // Serial.print(desiredSteps);
        // Serial.println("steps)");
    }

    // if we need to move physical steps
    if (stepperDrive->getStepsRemaining() != 0)
    {
        isMove = true;
        stepperDrive->myISR();
    } else {
        isMove = false;
    }
}



#endif // __CORE_H
