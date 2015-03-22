# tilt-gimbal
Arduino code for use with an Arduino Pro Mini which interprets a high/low auxiliary RC switch and smoothly tilts a brushless gimbal up/down.

Arduino code for a RC level translation module. My Turnigy TX/RX has an aux switch for controlling the 			
brushless gimbal, but there are a few problems: the actual high/low levels are not correct (the gimbal does		
not know how to interpret them), and the change between on (pointing straight down) and off (straight ahead)	
is abrubt and jerky. So, this code a:translates the high/low levels to ones the gimbal can recognize, and b:	
tilts the gimbal/camera up or down in a smooth fashion.															
																												
Inputs/Outputs:																									
Pin 2: RC Aux input channel signal (white)																		
Pin 3: Gimbal tilt output channel (white)																		
+5V from RX module																								
Common GND to/from both the reciever and the gimbal circuitry. 