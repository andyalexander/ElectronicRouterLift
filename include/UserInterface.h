#ifndef __USERINTERFACE_H
#define __USERINTERFACE_H

#include <Arduino.h>
#include "ControlPanel.h"
#include "Core.h"
// #include "Tables.h"

typedef struct MESSAGE
{
    char message[16];
    uint32_t displayTime;
    const MESSAGE *next;
} MESSAGE;

class UserInterface
{
private:
    ControlPanel *controlPanel;
    Core *core;

    KEY_REG keys;

    double heightCurrent;
    double heightTarget;
    double heightPrevious;

    double heightStep;              // step size

    bool isReady;                    // is the system ready for an input

    unsigned long startMicros;      // start time of ISR 
    int32_t startSteps;             // total steps to move at start

    const MESSAGE *message;
    uint16_t messageTime;

    LED_REG calculateLEDs();
    void setMessage(const MESSAGE *message);
    void overrideMessage( void );
    void updateLED(void);

    void undo(void);

public:
    UserInterface(ControlPanel *controlPanel, Core *core);

    void loop( void );
};

inline void UserInterface :: updateLED(void)
{
    // update the led
    LED_REG leds = this->calculateLEDs();
    controlPanel->setLEDs(leds);
}


#endif // __USERINTERFACE_H
