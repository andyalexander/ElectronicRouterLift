#include <Arduino.h>

#include <TM1638plus.h>

#include "Configuration.h"
#include "ControlPanel.h"
#include "StepperDrive.h"

#include "Core.h"
#include "UserInterface.h"


const unsigned long LOOP_DELAY_MICROS = 100000; //(1000 / UI_REFRESH_RATE_HZ) * 1000;
const unsigned long  ISR_DELAY_MICROS = 150;
unsigned long lastTimeISR;
unsigned long lastTimeUI;

// Dispay
TM1638plus display(TM_STROBE, TM_CLOCK, TM_DIO, false);

// Control Panel driver
ControlPanel controlPanel(&display);

// Stepper driver
StepperDrive stepperDrive;

// Core engine
Core core(&stepperDrive);

// User interface
UserInterface userInterface(&controlPanel, &core);


void setup() {

  Serial.begin(250000);

  lastTimeISR = micros();
  lastTimeUI = micros();

  // Initialize peripherals and pins
  controlPanel.initHardware();
  stepperDrive.initHardware();

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(SENSOR_TOP, INPUT_PULLUP); 
  pinMode(SENSOR_BIT, INPUT_PULLUP);
}

void loop() {
  unsigned long m = micros();

  // service the user interface
  // Serial.println((m-lastTimeUI)>LOOP_DELAY_MICROS);
  if (m<lastTimeUI || (m-lastTimeUI)>LOOP_DELAY_MICROS)
  {
    userInterface.loop();    
    lastTimeUI = m; 
  }
  
  
  if (m<lastTimeISR || m-lastTimeISR>ISR_DELAY_MICROS)
  {
    lastTimeISR = m;
    core.myISR();
  }
 
}



