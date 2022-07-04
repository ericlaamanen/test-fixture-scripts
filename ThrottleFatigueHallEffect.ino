// Uses hall effect sensor to count revolutions
// 3144 sensor output is high by default and goes low if magnetic field is detected
// Pull-up resistor between Vcc and digital output keeps output voltage high when no magnet is detected

#include <Wire.h>
#include <LiquidCrystal_I2C.h> 

#define INT 2 // hall effect sensor interrupt pin
#define PWM 5 // motor driver PWM pin
#define IN1 6 // motor driver input pin1
#define IN2 7 // motor driver input pin2

volatile int cycles = 0;
int cycle_set = 5000;
const unsigned long lockoutTime = 400; // debounce; time required between magnet detections to count as a cycle
unsigned long lastInterruptTime = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(4,0);
  
  Wire.begin();
  
  pinMode(INT,INPUT);
  pinMode(PWM,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  
  // digital interrupt. (pin to attach to, function to call, mode in which we want interrupt to work)
  attachInterrupt(digitalPinToInterrupt(INT), magnetDetect, FALLING);
}

void loop() {
    if (cycles < cycle_set){ // continue if current cycle number is less than set number of cycles
      int pwr = 65;
      int dir = 1;
      setMotor(dir, pwr, PWM, IN1, IN2);
   
      lcd.clear();
      lcd.backlight();
      //lcd.setCursor(0,0);
      //lcd.print("Cycles: ");
      lcd.setCursor(1,0);
      lcd.print(cycles);
    }
    
    setMotor(0,0,PWM,IN1,IN2); // If number of revs is greater than cycle set, then turn motor off
}

void setMotor(int dir, int pwmVal, int pwm, int in1, int in2) {
  analogWrite(pwm,pwmVal); // write PWM value between 0-255 on motor driver PWM pin
  if(dir == 1){
    digitalWrite(in1,HIGH); // in1 high and in2 low = CW rotation
    digitalWrite(in2,LOW);
  }
  else if(dir == -1){
    digitalWrite(in1,LOW); // in1 low and in2 high = CCW rotation
    digitalWrite(in2,HIGH);
  }
  else{
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
  }
}

void magnetDetect() { // debounce function for preventing false cycle counts as hall sensor transitions between high and low
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime >= lockoutTime){ 
     cycles++;
     lastInterruptTime = millis();
     Serial.println(cycles);
  }
}
