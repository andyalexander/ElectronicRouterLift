# Electronic Router Lift
This is a project heavily based on the awesome work by Clough42 and his ELS project.

I give all the credit for the code structure to him - having a generic framework for developing UI + hardware + interface projects is a great asset, and this goes a long way to helping.

I am based in the UK, so everthing here will assume you want to live in the metric world, measure in mm and can't work anything in inches!

## Project aim
I own a lot of tools, and I like to tinker.  Combined they led to this project.

The Electronic Router Lift (ERL) enables a generic manually cranked routerlift to be used with a stepper motor and digital control.  This gives a level of precision and repeatability that isn't available to most of us with manual setting.

## Initial version
This initial version is a PoC.  
* It has no encoder to determine the actual position of the screw, it just assumes the stepper will drive the screw to the position you set. 
* The lack of encoder, means that we assume zero backlash.  This is very unlikely to be true so going up->down will not be a repeatable process. 
* The logic for setting and resetting the height is still WIP, so no guarantees it won't do something odd depending on the sequence of use


## Hardware details
In case useful, some details of my hardware / router setup are below
* I use an ATMEL ICE debug probe to load to the devices, if you just use standard USB / serial you will need to edit the platformio.ini file; I have also hard coded my USB port so you will need to change this too ;)
* Routerlift thread pitch = 1/32"
* Stepper: NEMA 14
* Arduino: NANO (clone)
* Stepper driver: A4988 stepper driver (with heatsink) - these are good up to about 2a which is about right for my NEMA14 motor
* Pulley and belt: HTD 15mm, 3mm pitch

* small diam = 16mm; large diam = 45; belt = 261 mm  -> 82mm c2c
* small = 18T  (24t)        large = 48t  -> drive ratio 3:1

* NEMA 14 size:  holes c2c = 26   case = 35   core = 24
* NEMA 17 size:  holes c2c = 34   case = 42   core = 30

* NEMA 17: 1 = BLK, 2 = GRN, 3 = RED, 4 = BLU.  


To calculate the belt length, this is a pretty good calculator (https://www.omnicalculator.com/physics/belt-length)


Since the stepper driver assumes 5v and the arduino is 3.3v logic, we also need a level converter.  I've just used a £1 eBay special.  

Depending on feedback I get, I may follow Clough42's model and make a custom PCB to host the components / simplify the build.  

## Implementation details / caveats
* I don't use a hardware interrupt - it keeps the logic simpler as I know the 'ISR loops' will complete so memory management is easier (they aren't really ISR loops I know)
* Due to this, adding more logic changes the speed of loop execution, I look the pulses per second (pps) each time you press GO, so you can adjust the delay (main.cpp) to compensate if you do anything different.  Most steppers give some power response curve so you can tune the pps to the torque you need.

## Future ideas
* Add a contact probe for levelling - bar of metal laid over the opening, raise the lift till it touches and completes a circuit; zero level set :)
* Add limit switches so we can't do anything stupid...  probably Hall Effect sensors, as mechanical switch + dust = bad

## Commercial versions
In case anyone wants to buy not build, here are some links to commercial versions
* https://www.youtube.com/watch?v=I2CBChENtM8

