#include <Arduino.h>
#include <TM1638plus.h>
#include "ControlPanel.h"
#include "Configuration.h"


ControlPanel :: ControlPanel(TM1638plus* display)
{
    this->display = display;
    this->leds.all = 0;
    this->keys.all = 0;
    this->message = NULL;
    this->brightness = 0x02;

    this->heightDelta = 0.0;
}

void ControlPanel :: initHardware(void)
{
    this->display->displayBegin();
    Serial.println("Display initialised...");
}

void ControlPanel :: sendData()
{
    uint16_t ledMask = this->leds.all;

    // uint16_t briteVal = 0x80;
    // if( this->brightness > 0 ) {
    //     this->display->brightness(this->brightness);
    // }

    char values1[6];  // 4 chars + decimal point + terminator
    char values2[6];
  
    // snprintf(values1, 5, "%2f", 3.7); 
    dtostrf(this->heightCurrent+this->heightDelta,5,1,values2);             // rhs shows the total height after move
    dtostrf(this->heightDelta,5,1,values1);                                 // lhs shows the delta

    // if there is a message display it
    if( this->message != NULL ){
        this->display->displayText(this->message);
    }
    // otherwise display the lhs / rhs info
    else {
        this->display->displayText(strcat(values1, values2));
    }

    // update the LEDs
    for (uint8_t LEDposition = 0; LEDposition < 8; LEDposition++) {
        this->display->setLED(LEDposition, ledMask & 1);
        ledMask = ledMask >> 1;
    }

}

KEY_REG ControlPanel :: readKeys(void)
{
    KEY_REG keyMask;
    keyMask.all = this->display->readButtons();

    return keyMask;
}

KEY_REG ControlPanel :: getKeys()
{
    KEY_REG newKeys;
    static KEY_REG noKeys;

    newKeys = readKeys();
    if( newKeys.all != this->keys.all ) {
        this->keys = newKeys;
        return newKeys;
    }
    return noKeys;
}

void ControlPanel :: setMessage( const char *message )
{
    this->message = message;
}

void ControlPanel :: setBrightness( uint16_t brightness )
{
    if( brightness > 8 ) brightness = 8;

    this->brightness = brightness;
}

void ControlPanel :: refresh()
{
    sendData();
}






