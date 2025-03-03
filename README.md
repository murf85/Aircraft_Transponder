# Aircraft_Transponder
DEVS Model of and aircraft transponder interrogation and reply

Introduction
This repository contains a DEVS model of an Aircraft Transponder.  In addition to the top model, there are two coupled models representing a radar and an aircraft. 
The radar sends interrogation pulses to the aircraft, and the aircraft replies with Mode A code or Mode C altitude.   

Dependencies
This project assumes that you have Cadmium installed in a location accessible by the environment variable $CADMIUM. 

Build
To build this project, run:

source build_sim.sh

NOTE: Everytime you run build_sim.sh, the contents of build/ and bin/ will be replaced.

Execute
To run the models in this project, run:

./bin/aircraft_transponder_top

NOTE: Remember to change the file path to the testcase files in /main/include/aircraft_top_model.hpp or it will result in a fault.
