/* stepper guage command test

serial signal is sent to Serial1 instead of default

Wallace Chen
*/

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);           // set up Serial library at 9600 bps
}

void loop() {
  int output = 300;
  Serial.println("start");
  Serial1.println(500.3);
  delay(1000);
  Serial1.println(100);
  delay(1000);
  Serial1.println(1000);
  delay(1000);
  
  Serial1.println(300.5);
  delay(1000);
  
  Serial1.println(700);
  delay(1000);
  
  Serial1.println(0.0);
  delay(1000);
  
  Serial.println("end");
}
