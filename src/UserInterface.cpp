#include "UserInterface.h"

// const MESSAGE STARTUP_MESSAGE_2 =
// {
//   "1.0.0",
//   (uint32_t)(UI_REFRESH_RATE_HZ * 1.0),
//   NULL
// };

const MESSAGE STARTUP_MESSAGE_1 =
{
    "E.r.L  1.0.0",
    (uint32_t)(UI_REFRESH_RATE_HZ * 1.5),
    NULL
    // &STARTUP_MESSAGE_2
};

//UserInterface :: UserInterface(ControlPanel *controlPanel, Core *core, FeedTableFactory *feedTableFactory)
UserInterface :: UserInterface(ControlPanel *controlPanel, Core *core)
{
    this->controlPanel = controlPanel;
    this->core = core;

    this->heightCurrent = 0;                  // start from zero
    this->heightTarget = this->heightCurrent;
    this->heightPrevious = 999.9;
    this->heightStep = HEIGHT_STEP_LARGE;

    this->isReady = this->core->isReady();

    this->keys.all = 0xff;

    setMessage(&STARTUP_MESSAGE_1);
    this->controlPanel->setHeightCurrent(this->heightCurrent);
    
    // update the led
    LED_REG leds = this->calculateLEDs();
    controlPanel->setLEDs(leds);
}

LED_REG UserInterface::calculateLEDs()
{
    // get the LEDs for this feed
    LED_REG leds;

    // and add a few of our own
    leds.bit.POWER = this->core->getPowerState();
    // leds.bit.LIMIT = this->core->getLimitState();
    leds.bit.STEP_BIG = this->heightStep == HEIGHT_STEP_LARGE;
    leds.bit.STEP_SMALL = !leds.bit.STEP_BIG;
    leds.bit.READY = this->isReady;
    // leds.bit.READY = this->core->isReady();

    return leds;
}

void UserInterface :: setMessage(const MESSAGE *message)
{
    this->message = message;
    this->messageTime = message->displayTime;
}

void UserInterface :: overrideMessage( void )
{
    if( this->message != NULL )
    {
        if( this->messageTime > 0 ) {
            this->messageTime--;
            controlPanel->setMessage(this->message->message);
        }
        else {
            this->message = this->message->next;
            if( this->message == NULL )
                controlPanel->setMessage(NULL);
            else{
                this->messageTime = this->message->displayTime;
            }
        }
    }
    else{
    }
}

void UserInterface :: undo( void )
{
    // Serial.println("Undo...");
    // if (this->heightPrevious != 999.9 && this->isReady)
    // {
        this->heightCurrent = this->heightPrevious;
        this->heightTarget = this->heightCurrent;
        controlPanel->setHeightDelta(0.0);
        controlPanel->setHeightCurrent(this->heightCurrent);
        this->heightPrevious = 999.9;
    // }
}

void UserInterface :: loop( void )
{
    // display an override message, if there is one
    overrideMessage();

    // ready state changed
    if (this->isReady != core->isReady())
    {
        this->isReady = this->core->isReady();
        this->updateLED();

        if (core->getStepsRemaining() == 0)
        {
            unsigned long m = micros() - this->startMicros;
            Serial.print("Done in: ");
            Serial.print(m);
            Serial.print(" (");
            Serial.print( round(1000000/(m/abs(this->startSteps))) );
            Serial.println(" pps)");

            this->heightCurrent = this->heightTarget;
            this->heightPrevious = 999.9;
            controlPanel->setHeightDelta(0.0);
            controlPanel->setHeightCurrent(this->heightCurrent);
        }
    }
    

    // read keypresses from the control panel
    keys = controlPanel->getKeys();

    // respond to keypresses
    if( keys.bit.SET_ZERO)
    {
        // Serial.println("Zero...");
        // if (this->isReady)
        if (this->core->isReady())
        {
            this->heightCurrent = 0.0;
            this->heightPrevious = 999.9;
            this->heightTarget = this->heightCurrent;
            controlPanel->setHeightDelta(0.0);
            controlPanel->setHeightCurrent(this->heightCurrent);
        }
    }

    if (keys.bit.TOGGLE_STEP)
    {
        // Serial.println("Press: toggle step");
        // Serial.println(this->heightStep);

        if ( this->heightStep == HEIGHT_STEP_LARGE ) 
        {
            this->heightStep = HEIGHT_STEP_SMALL;
        } else 
        {
            this->heightStep = HEIGHT_STEP_LARGE;
        }

        this->updateLED();
    }

    // if (keys.bit.UNDO)
    // {
            // this->undo();
    // }
    
    if( keys.bit.UP )
    {
        // if (this->isReady)
        if (this->core->isReady())
        {
            if (this->heightPrevious == 999.9){this->heightPrevious = this->heightCurrent;}

            this->heightTarget += this->heightStep;
            controlPanel->setHeightDelta(this->heightTarget - this->heightCurrent);
            controlPanel->setHeightCurrent(this->heightCurrent);
        }
    }
    if( keys.bit.DOWN )
    {
        // if (this->isReady)
        if (this->core->isReady())
        {
            if (this->heightPrevious == 999.9){this->heightPrevious = this->heightCurrent;}

            this->heightTarget -= this->heightStep;
            controlPanel->setHeightDelta(this->heightTarget - this->heightCurrent);
            controlPanel->setHeightCurrent(this->heightCurrent);
        }
    }

    if( keys.bit.POWER)
    {
        // if already on, then undo, and turn off
        if (core->getPowerState()){
            this->undo();
        }
        core->powerToggle();
        this->updateLED();
    }

    if ( keys.bit.GO_TARGET)
    {
        // if (this->isReady && this->heightTarget != this->heightCurrent)
        if (this->core->isReady() && this->heightTarget != this->heightCurrent)
        {
            this->isReady = false;
            core->setHeightDelta(this->heightTarget - this->heightCurrent);

            this->updateLED();  

            Serial.print("Go...");
            this->startMicros = micros();
            this->startSteps = core->getStepsFromHeight(this->heightTarget - this->heightCurrent);

        }    
    }

    // write data out to the display
    controlPanel->refresh();
}
