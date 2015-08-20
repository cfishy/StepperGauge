/* 
Open Source Stepper Gauge 

Wallace Chen
*/

// overwrite me ifdef
// adafruit stepper motor shield
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


//rfrobot lcd shield
#include <LiquidCrystal.h>
 LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel

//todo: doc here
char direction = 's'; //stop 
int stepper_position = 0;
int target_position = 0;
int max_steps = 10;
int min_steps = -10;  

// overwrite me ifdef
//abstract layer for stepper controls
int step_up(int steps) {
     myMotor->step(steps, FORWARD, MICROSTEP);
   return steps;
}

// overwrite me ifdef
//abstract layer for stepper controls
int step_down(int steps) {
      myMotor->step(abs(steps), BACKWARD, MICROSTEP);
     return steps;
}

// s positive is forward num of steps, neg is backward
int do_step(int s) {
  if (s > 0) {
    return step_up(s);
  } else if (s < 0) {
    return step_down(s);
  }
}

int update_step(int curr_position, int taget_position) {
  return curr_position + do_step(target_position - curr_position);
}



void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  
  //stepper init
  AFMS.begin();  // create with the default frequency 1.6KHz
  myMotor->setSpeed(10);  // 10 rpm   
  
  //LCD init
   lcd.begin(16, 2);               // start the library
   lcd.setCursor(0,0);             // set the LCD cursor   position 
   lcd.print("ready");
   
}


void loop() {
  if (Serial.available()) {
    target_position = Serial.parseInt(); //steps, positive in is forward
    
    //cut off target if out of bounds
    if (target_position > max_steps) target_position = max_steps;
    if (target_position < min_steps) target_position = min_steps;
    
    stepper_position = update_step(stepper_position, target_position);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(stepper_position);
  }
}
