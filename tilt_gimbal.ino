/*
=====================================================================================================================
|	Arduino code for a RC level translation module. My Turnigy TX/RX has an aux switch for controlling the 			|
|	brushless gimbal, but there are a few problems: the actual high/low levels are not correct (the gimbal does		|
|	not know how to interpret them), and the change between on (pointing straight down) and off (straight ahead)	|
|	is abrubt and jerky. So, this code a:translates the high/low levels to ones the gimbal can recognize, and b:	|
|	tilts the gimbal/camera up or down in a smooth fashion.															|
|																													|
|	Inputs/Outputs:																									|
|	Pin 2: RC Aux input channel signal (white)																		|
|	Pin 3: Gimbal tilt output channel (white)																		|
|	+5V from RX module																								|
|	Common GND to/from both the reciever and the gimbal circuitry.                                                	|
=====================================================================================================================
*/

//Use the arduino servo library to make this a little easier.
#include <Servo.h>

//Declare the servo object
Servo myservo;

//Define some pins
#define IN_PIN 2
#define LED 13
#define CENTER 95
#define DOWN 45

//Declare variables
unsigned long duration = 0;
int pos = 0;
int currentPos = 0;
int lastPos = 0;

void setup() {
	//Open the serial port for USB debugging
	Serial.begin(9600);
	//attaches the servo on pin 3 
	myservo.attach(3);  

	//Set the IO pinmodes
	pinMode(IN_PIN, INPUT);
	pinMode(LED, OUTPUT);

	//Turn the status LED off
	digitalWrite(LED, LOW);

	//Initial positions
	currentPos = CENTER;
	lastPos = CENTER;
}

void loop() {
	//Using pulseIn to read the length of the RC pulse. ~1000us dictates low, ~2000us dictates high.
	//Take a 5 sample average so that any jitter doesn't completely throw off the readings.
	duration = 0;
	for (int j=0; j<5; j++) {
		duration += pulseIn(IN_PIN, HIGH);
	}
	duration /= 5;
	Serial.println(duration);

	//If duration is higher than 1500, set the gimbal to tilt down.
	if (duration > 1500) {
		currentPos = DOWN;
		digitalWrite(LED, HIGH); //Turn the LED on to show that the command has been recieved.
	}

	//If it isn't pointing down, it's probably pointing up!
	else {
		currentPos = CENTER;
		digitalWrite(LED, LOW);
	}

	if (currentPos != lastPos) {
		//The position has changed, so move the gimbal either up or down.
		//Use a switch to determine which position the gimbal should be in.
		switch (currentPos) {
		    case DOWN:
		      //Move the gimbal down smoothly.
		      for(pos = CENTER; pos > DOWN; pos -= 1)     
		      // Goes from 180 degrees to 0 degrees 
				{         
			  	//set the servo positon                       
			    myservo.write(pos); 
			    //Use a small delay so that the camera tilts smoothly.
			    delay(100);                 
				} 
				//Keep track of the current position. It's down now.
				lastPos = DOWN;
		      break;

		    case CENTER:
		      // move the gimbal to center, much faster up than down, because it looks cool.
		      for(pos = DOWN; pos < CENTER; pos += 1)
		      	// goes from 0 degrees to 180 degrees 
			  	{    
			  	//set the servo position                            
			    myservo.write(pos);
			    //Use a small delay so that the camera tilts smoothly.
			    delay(10);
				} 
				//Keep track of the current position. It's up now.
				lastPos = CENTER;
		      break;

		    default:
		      // do nothing. Meow!
		      break;
		}
	}
}
