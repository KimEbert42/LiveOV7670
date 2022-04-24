



// change setup.h to switch between buffered and pixel-by-pixel processing
#include "setup.h"


// Include the servo library:
#include <Servo.h>

// Create a new servo object:
Servo myservo;

// Define the servo pin:
#define servoPin 8

// Create a variable to store the servo position:
int angle = 0;

int servoDelay = 5;

void setup() {

  myservo.attach(servoPin);

  // give time for power draw to settle
  delay(10000);
  myservo.write(35);
  delay(1000);

  // This is not necessary and has no effect for ATMEGA based Arduinos.
  // WAVGAT Nano has slower clock rate by default. We want to reset it to maximum speed
  CLKPR = 0x80; // enter clock rate change mode
  CLKPR = 0; // set prescaler to 0. WAVGAT MCU has it 3 by default.

  initializeScreenAndCamera();
}


void loop() {
  if (servoDelay == 0) { 
    myservo.write(90);
    delay(1000);
  }
  
  processFrame();
}
