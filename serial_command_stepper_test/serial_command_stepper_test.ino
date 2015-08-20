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
int stepper_position = 0;
int target_position = 0;
int max_steps = 10;
int min_steps = -10;  
int max_input = 1024;
int min_input = 0;

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


// linear distribution of input to range of steps
int get_position(int input) {
  
  // cut off if input is out of bounds
  float steps_per_input = 0;
  if (input < min_input) {
    input = min_input;
  } else if (input > max_input) {
    input = max_input;
  }
  
  // how many steps per one input increase? 
  steps_per_input = (float)(max_steps - min_steps) / (float)(max_input - min_input);
  return (int)(input * steps_per_input);
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
  int input = 0;
  if (Serial.available()) {
    input = Serial.parseInt(); //steps, positive in is forward
    target_position = get_position(input);
    
    //debug
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(target_position);
    
    
    //move the stepper
    stepper_position = update_step(stepper_position, target_position);
    
    //display
    //lcd.clear();
//    lcd.setCursor(0,0);
//    lcd.print(stepper_position);
  }
}
