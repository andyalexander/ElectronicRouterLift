#ifndef __CONTROL_PANEL_H
#define __CONTROL_PANEL_H

#include <Arduino.h>
#include <TM1638plus.h>

// #define LED_READY (1<<7)
// #define LED_LIMIT (1<<6)
// #define LED_6 (1<<5)
// #define LED_5 (1<<4)
// #define LED_4 (1<<3)
// #define LED_3 (1<<2)
// #define LED_2 (1<<1)
// #define LED_POWER (1)

struct LED_BITS
{
    uint16_t POWER:1;
    uint16_t LIMIT:1;
    uint16_t STEP_BIG:1;
    uint16_t STEP_SMALL:1;
    uint16_t LED5:1;
    uint16_t LED6:1;
    uint16_t LED7:1;
    uint16_t READY:1;
};

typedef union LED_REG
{
    uint16_t all;
    struct LED_BITS bit;
} LED_REG;

struct KEY_BITS
{
    uint16_t POWER:1;
    uint16_t FULL_RAISE:1;
    uint16_t TOGGLE_STEP:1;
    // uint16_t UNDO:1;
    uint16_t SET_ZERO:1;
    uint16_t GO_TARGET:1;
    uint16_t DOWN:1;
    uint16_t BUTTON7:1;
    uint16_t UP:1;
    };

typedef union KEY_REG
{
    uint16_t all;
    struct KEY_BITS bit;
} KEY_REG;


class ControlPanel
{
private:
    // Display
    TM1638plus *display;

    //  height
    double heightCurrent;       // current height
    double heightDelta;        // adjustment

    // Current LED states
    LED_REG leds;

    // current key states
    KEY_REG keys;

    // current override message, or NULL if none
    const char *message;

    // brightness, levels 1-8, 0=off
    uint16_t brightness;

    KEY_REG readKeys(void);
    uint16_t lcd_char(uint16_t x);
    void sendByte(uint16_t data);
    uint16_t receiveByte(void);
    void sendData(void);
    uint16_t reverse_byte(uint16_t x);

public:
    ControlPanel(TM1638plus *display);

    // initialize the hardware for operation
    void initHardware(void);

    // poll the keys and return a mask
    KEY_REG getKeys(void);

    // set the values to display
    void setHeightCurrent(double height);
    void setHeightDelta(double height);
  
    // set the LED states
    void setLEDs(LED_REG leds);

    // set a message that overrides the display, 8 characters required
    void setMessage(const char *message);

    // set a brightness value, 0 (off) to 8 (max)
    void setBrightness(uint16_t brightness);

    // toggle the height step size
    // void toggleHeightStep();

    //get the height step
    // double getHeightStep();

    // refresh the hardware display
    void refresh(void);
};


inline void ControlPanel :: setHeightCurrent(double height)
{
    this->heightCurrent = height;
}

inline void ControlPanel :: setHeightDelta(double height)
{
    this->heightDelta = height;
}

inline void ControlPanel :: setLEDs(LED_REG leds)
{
    this->leds = leds;
}


#endif // __CONTROL_PANEL_H
