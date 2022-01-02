#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#define LEADSCREW_TPI 32  // approx 0.8mm pitch
#define MM_PER_INCH 25.4

//================================================================================
//                                STEPPER/SERVO
//
// Define the number of steps and microsteps for your stepper motor, the pin
// polarity of the driver, and whether to use additional features, like servo
// alarm feedback.
//
// NOTE: If you are using a servo drive with something other than a 1:1 drive
// ratio, you can use the STEPPER_MICROSTEPS to compensate.  For example, if you
// have a servo configured for 1000 steps/revolution and it drives the leadscrew
// through a 3:1 reduction, you can set STEPPER_RESOLUTION to 1000 and
// STEPPER_MICROSTEPS to 3.
//================================================================================

// Steps and microsteps
#define STEPPER_MICROSTEPS 3
#define STEPPER_RESOLUTION 1000

// Step, direction and enable pins are normally active-high
#define INVERT_STEP_PIN true
#define INVERT_DIRECTION_PIN true
#define INVERT_ENABLE_PIN true
#define INVERT_ALARM_PIN true

// Enable servo alarm feedback
#define USE_ALARM_PIN

// the step for each height change
#define HEIGHT_STEP_SMALL 0.1
#define HEIGHT_STEP_LARGE 1.0

#define DISPLAY_BRIGHTNESS 5



//================================================================================
//                               CPU / TIMING
//
// Define the CPU clock, interrupt, and refresh timing.  Most users will not need
// to touch these settings.
//================================================================================

// Stepper state machine cycle time, in microseconds
// Two cycles are required per step
// #define STEPPER_CYCLE_US 5
#define STEPPER_CYCLE_US 50

// User interface refresh rate, in Hertz
#define UI_REFRESH_RATE_HZ 10

// RPM recalculation rate, in Hz
#define RPM_CALC_RATE_HZ 100
#define RPM_CALC_INTERVAL 10 // delay in ms

// Microprocessor system clock
#define CPU_CLOCK_MHZ 84                            // due is 84 mhz
#define CPU_CLOCK_HZ (CPU_CLOCK_MHZ * 1000000)


#endif // __CONFIGURATION_H