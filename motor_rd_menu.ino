int pwm_a = 3;
int pin_dir_a = 12;
int pwm_b = 11;
int pin_dir_b = 13;
int pin_collision_front = 2;
const int FORWARD = 1;
const int BACKWARD = 0;
const int collision_status = 7;

int direction_a = FORWARD;
int direction_b = FORWARD;
int global_speed =90;


void setup() {
  Serial.begin(9600);
  pinMode(pwm_a, OUTPUT);
  pinMode(pwm_b, OUTPUT);
  pinMode(pin_dir_a, OUTPUT);
  pinMode(pin_dir_b, OUTPUT);
  pinMode(pin_collision_front, INPUT);
  pinMode(collision_status, OUTPUT);
  setMotorADirection(FORWARD);
  setMotorBDirection(FORWARD);
  stopMoving();
  //blink col status LED to say "ready"
  for(int i=0; i<5; i++){
    digitalWrite (collision_status, HIGH);
    delay(200);
    digitalWrite (collision_status, LOW);
    delay(200);
  }
  Serial.println("");
  Serial.println("Ready");
  attachInterrupt(0, collision_front, CHANGE);
  

  // put your setup code here, to run once:

}


void collision_front() {
  int r_state = digitalRead(pin_collision_front);    // reads the status of the sensor
  //coll_front_detected = r_state;
  if(r_state == 0)
    digitalWrite (collision_status, HIGH);      // collision detectet, turn on the led
  else
    digitalWrite (collision_status, LOW);       // turn off the led
  //stopMoving(); 
//  cli();
  stopMoving(); 
}

void setMotorADirection(int dir) {
  digitalWrite(pin_dir_a, dir);
  direction_a = dir;
}

void setMotorBDirection(int dir) {
  digitalWrite(pin_dir_b, dir);
  direction_b = dir;
}

void switchDirection() {
  direction_a = !direction_a;
  direction_b = !direction_b;
  setMotorADirection(direction_a);
  setMotorBDirection(direction_b);

}

void setSpeedMotorA(unsigned int speed) {
  digitalWrite(pwm_a,speed);
}

void setSpeedMotorB(unsigned int speed) {
  digitalWrite(pwm_b,speed);
}

void stopMotorA(){
  setSpeedMotorA(0);
}

void stopMotorB(){
  setSpeedMotorB(0);
}

void moveForward() {
  setMotorADirection(FORWARD);
  setMotorBDirection(FORWARD);
  setSpeedMotorA(global_speed);
  setSpeedMotorB(global_speed);
  
}

void moveBackward() {
  setMotorADirection(BACKWARD);
  setMotorBDirection(BACKWARD);
  setSpeedMotorA(global_speed);
  setSpeedMotorB(global_speed);
}

void turnRight() {
  //stopMotorA();
  setMotorBDirection(FORWARD);
  setMotorADirection(BACKWARD);
  setSpeedMotorB(global_speed);
  setSpeedMotorA(global_speed);  
}

void turnLeft() {
  //stopMotorB();
  setMotorADirection(FORWARD);
  setMotorBDirection(BACKWARD);
  
  setSpeedMotorB(global_speed);
  setSpeedMotorA(global_speed);  
  
     
}

void stopMoving() {
  stopMotorA();
  stopMotorB();
}




void printMenu(){
  Serial.println("(s)top (f)orward (b)ackward (l)eft (r)ight");
}


int command_found;

void loop() {
  
  // put your main code here, to run repeatedly: 
  // while(Serial.available()){
  //  Serial.read();
  //}
  
  //wait till data is available on serial port
  //Serial.println("Press a key to start me movin (again)...");
  printMenu();
  
  //wait for key
  while( !Serial.available()  >0) {};
  // read next key
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    command_found = 1; //assume valid cmd as default;
    
    // do something different depending on the character received.  
    // The switch statement expects single number values for each case;
    // in this exmaple, though, you're using single quotes to tell
    // the controller to get the ASCII value for the character.  For 
    // example 'a' = 97, 'b' = 98, and so forth:

    switch (inByte) {
    case 'f':
      Serial.println("Forward");    
      moveForward();
      break;
    case 'b':
      Serial.println("Backward");    
      moveBackward();
      break;
    case 'l':   
      Serial.println("Left"); 
      turnLeft();
      break;
    case 'r':
      Serial.println("Right");    
      turnRight();
      break;
    case 's':
      Serial.println("Stop");    
      stopMoving();
      break;
    default:
      Serial.println("Unknown command");
      command_found=0;
      
    } 
  if(command_found) {
    delay(1000);
    stopMoving();
  }
  //Serial.println("Back to start");
}
}
