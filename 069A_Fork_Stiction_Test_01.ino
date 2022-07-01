#include <AccelStepper.h>
#include <Stepper.h>

#define dirPin 5  
#define stepPin 4
#define motorInterfaceType 1
#define runButtonPin 8
#define forwardButtonPin 9
#define reverseButtonPin 10

int maxMotorSpeed = 720;
int motorTestSpeed = 200;
int motorAccel = 8000;
long endTestPosition = 3200;
bool runButtonState = HIGH;
bool forwardButtonState = HIGH;
bool reverseButtonState = HIGH;
long maxMovePerPress = 50000;

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  
  
  pinMode(runButtonPin,INPUT_PULLUP); // INPUT_PULLUP uses internal pull-up resistor to keep pin state HIGH
  pinMode(forwardButtonPin,INPUT_PULLUP);
  pinMode(reverseButtonPin,INPUT_PULLUP);
  
  stepper.setMaxSpeed(maxMotorSpeed); // Set max speed for motor
  stepper.setSpeed(motorTestSpeed); // Set speed for test
  stepper.setAcceleration(motorAccel); // Set motor acceleration
}

void loop() {

  // if run button has been pressed call function to run test
  if (digitalRead(runButtonPin) == LOW) {
    runTest();
  }

  // if forward jog button has been pressed call function to jog forward
  if (digitalRead(forwardButtonPin) == LOW) {
    stepper.moveTo(maxMovePerPress);
    forwardJog(); 
  }

  // if reverse jog button has been pressed call function to jog backwards
  if (digitalRead(reverseButtonPin) == LOW) {
    stepper.moveTo(-maxMovePerPress);
    reverseJog(); 
  }
}

void runTest() {    

     delay(100);
     stepper.setCurrentPosition(0); // Zero position
     stepper.moveTo(endTestPosition); // Set destination as distance required for test
      
     stepper.runToPosition(); // Run to target position with set speed and acceleration
    
     delay(5000); // Wait before reversing travel
      
     stepper.moveTo(0); // Return to zero
     stepper.runToPosition();
     stepper.disableOutputs(); // stops motor from overheating when off

     runButtonState = HIGH; // reset button state
}

void forwardJog() {

  // Read forward button pin and while pin is low (button pressed), if max travel hasn't been reached, jog forward.
  while (digitalRead(forwardButtonPin) == LOW) { 
    if (stepper.currentPosition() != maxMovePerPress) {  
      stepper.run();
    }
  } 
  stepper.setSpeed(0);
  stepper.setCurrentPosition(0);
  stepper.moveTo(stepper.currentPosition());
  stepper.disableOutputs(); 
}

void reverseJog() {

  // Read reverse button pin and while pin is low, if max travel hasn't been reached, jog reverse.
  while (digitalRead(reverseButtonPin) == LOW) {
    if (stepper.currentPosition() != -maxMovePerPress) {  
      stepper.run();
    }
  } 
  stepper.setSpeed(0);
  stepper.setCurrentPosition(0);
  stepper.moveTo(stepper.currentPosition()); 
  stepper.disableOutputs();
}








  
  
    
