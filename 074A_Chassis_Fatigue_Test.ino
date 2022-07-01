#include <LiquidCrystal.h>
int Sensor1safe = 1; // Position Sensor on air Piston - fixture 1
unsigned long x = 500; // Time threshold between clicks sensor 1 in milliseconds
int pr1 = 1;  //print for sensor 1
int firstrun = 1;
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;   //declaring the variables for data pins of the LCD screen
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // changing the pins so that the LCD works with Arduino UNO
​
void setup() {
  lcd.begin(16, 2); //starts the LCD screen... the screen size is 16 x 2
  pinMode(A1, OUTPUT);  //Sensor 1
  pinMode(A2, INPUT);  //Sensor 1
  lcd.begin(16, 2);
  lcd.print("RUN:");
}
​
void loop() {
​
  //  if (firstrun) {
  //    lcd.print("TEST IN PROGRESS");    //This tells the LCD to display "TEST IN PROGRESS" while the piston is running
  //    firstrun = 0;                     //This makes sure that its only printed once
  //  }
​
  if (digitalRead(A2) == HIGH) {
    lcd.setCursor(4,0);
    lcd.print(millis() / 1000);
//    lcd.print(" cycles");
  }
  else  {     //If the position sensor is tripped and displays its red LED then the variable called sensor safe is changed to 0 (not safe)
    //sensor 1 input reads a low signal then
    //make the integer Sensor1safe equal to zero
    if (pr1) ;
    {
//      lcd.clear();                //Clears the previous text on the display
      lcd.setCursor(0, 1);        //Changes the position of the cursor to the top line
      lcd.print("STOP: ");   //Prints on the top line
      lcd.print(x / 1000);
      lcd.setCursor(1,6);        //Change the cursor position to the bottom line
      //      lcd.print( x / 1000);       // Print the number of cycles that the test has a
//      lcd.print(" cycles");
      Sensor1safe = 0;
    }
    pr1 = 0;
​
  }
​
  if (millis() >= x && Sensor1safe) {             //if the time since the test has started is greater than or equal to x and Sensor1safe is good ...run this if statement
    if (x % 1000 == 0 ) {                     // if the remainder of x divided by 1000 is zero
      digitalWrite(A1, HIGH);                 // make the signal for the Relay HIGH
    }
    else {                                    // if not then
      digitalWrite(A1, LOW);                   // make the signal for the Relay LOW
    }
    x = x + 500;                      //increase the value of x by 500 millisecconds
  }                                  // turn the relay on (HIGH is the voltage level)
}
