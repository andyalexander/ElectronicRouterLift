# Electronic Router Lift
This is a project heavily based on the awesome work by Clough42 and his ELS project.

I give all the credit for the code structure to him - having a generic framework for developing UI + hardware + interface projects is a great asset, and this goes a long way to helping.

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
* I use an ATMEL ICE debig probe to load to the devices, if you just use standard USB / serial you will need to edit the platformio.ini file; I have also hard coded my USB port so you will need to change this too ;)
* Routerlift thread pitch = 1/32"
* Stepper: NEMA 14
* Arduino: NANO (clone)
* Stepper driver: 

Since the stepper driver assumes 5v and the arduino is 3.3v logic, we also need a level converter.  I've just used a £1 eBay special.  

Depending on feedback I get, I may follow Clough42's model and make a custom PCB to host the components / simplify the build.  

## Future ideas
* Add a contact probe for levelling - bar of metal laid over the opening, raise the lift till it touches and completes a circuit; zero level set :)
* Add limit switches so we can't do anything stupid...  probable Hall Effect sensors, as mechanical switch + dust = bad

