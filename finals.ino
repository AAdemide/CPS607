///finals
#include <avr/wdt.h>
#include "DeviceDriverSet_xxx0.h"
#include "ApplicationFunctionSet_xxx0.cpp"
#include <Servo.h> 

DeviceDriverSet_ITR20001 AppITR20001;
DeviceDriverSet_ULTRASONIC myUltrasonic;
DeviceDriverSet_Motor AppMotor;
Application_xxx Application_SmartRobotCarxxx0;

int left_line_sensor = 0;
int right_line_sensor = 0;
int flame_sensor = 0;
int servoPin = 11; 
int flame_pin = A4;
Servo Servo1; 
//70 40 nice
int og_forward_speed=40;
int forward_speed=og_forward_speed;
int turn_speed=70;
int obstacle_allowance=15; //for slowing/changing lanes 
int stop_dist = 10;
// we always start from the inside lane, false means go to outside lane true means go to inside lane
bool go_to_lane = false; 
bool flame_triggered = true;
int offline = 100;
int parkingslot_check = 0;

bool REDUCE_SPEED;
bool LINE_SENSOR_CLEAR;
bool TURN_LEFT;
bool TURN_RIGHT;

// unsigned int tempda_1 = 0;
// unsigned int tempda_2 = 0;
unsigned int tempda_3 = 0;
unsigned int tempda_4 = 0;
unsigned int tempda_5 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myUltrasonic.DeviceDriverSet_ULTRASONIC_Init();
  AppMotor.DeviceDriverSet_Motor_Init();
  Servo1.write(90);
  delay(1000);
  pinMode(left_pin, INPUT);
  pinMode(right_pin, INPUT);
  pinMode(flame_pin, INPUT);
  Servo1.attach(servoPin); 
}

void loop() {
  // put your main code here, to run repeatedly:

  ultrasonic_get();
  // printUltraSonic();
  line_sensor_get();
  flame_sensor_get();
  //print_line_sensor();
  //move(Forward, 40, 0);
  //line_sensing();
  //if (flame_sensor < 100){
    parking();
  //} else if (flame_triggered) 
  //  line_sensing();
}

void parking(){
  flame_sensor_get();
  
    if (flame_sensor < 100 && flame_triggered){
    flame_triggered = false;
    // print_flame_sensor();
    //move(Forward, turn_speed, 500);
    //move(Left, turn_speed, 1000);
    //move(Forward, turn_speed, 1300);
    //move(Left, turn_speed, 1000);
    } //delete after testing
    while (parkingslot_check < 2) {
      Serial.print(parkingslot_check);
      move(Forward, 60, 1000);
      if (tempda_5 > 20){
        parkingslot_check += 1;
      } else {
        parkingslot_check = 0;        
      }
      move(stop_it, 0, 2000);
    }
    if (parkingslot_check >= 2) digitalWrite(13, 1);
  //}
  move(stop_it, 0, 0);
}

void line_sensing(){
  if ( TURN_LEFT && flame_sensor > 100  ){ //&& flame_sensor > 100
    while(true){ 
      line_sensor_get();
      flame_sensor_get();
      if( LINE_SENSOR_CLEAR || flame_sensor < 100){ 
        break;
      }
      move(stop_it, 0, 0);
      move(Left, turn_speed, 0);
      //print_line_sensor();
    } 
  } else if ( TURN_RIGHT && flame_sensor > 100 ) {  //
    while(true){  
      line_sensor_get();
      flame_sensor_get();
      if( LINE_SENSOR_CLEAR || flame_sensor < 100){// || flame_sensor < 100
        break;
      }
      move(stop_it, 0, 0);
      move(Right, turn_speed, 0);        
      //print_line_sensor();
    }
  } else if ( LINE_SENSOR_CLEAR && flame_sensor > 100 )  {
    while(true){
      ultrasonic_get();
      line_sensor_get();
      flame_sensor_get();
      if(right_line_sensor > offline || left_line_sensor > offline || flame_sensor < 100)
        break;
      
      if( ( tempda_4 < obstacle_allowance && tempda_4 > stop_dist ) && forward_speed > 25){
        forward_speed-=1;
      }
      else if ( ( tempda_4 > obstacle_allowance ) && forward_speed < og_forward_speed)
        forward_speed+=1;
      else if ( (tempda_4 <= stop_dist) && forward_speed<og_forward_speed){
        forward_speed=og_forward_speed;
        //delay(100)
        //go to next lane  
        digitalWrite(13, 1);
        go_to_new_lane();        
        //digitalWrite(13, 1);
        //break;
      }
      // else{
      //   digitalWrite(13, 1);
      // }
      move(Forward, forward_speed, 0);
      //print_line_sensor();
    }
  }
}

void go_to_new_lane(){
  //servo checking
  //move(Backward, forward_speed, 100);
  int direction;
  if(!go_to_lane)
    direction=Left;
  else
    direction=Right;

  move(direction, turn_speed, 500);

  go_to_lane=!go_to_lane;
}

void move(int direction, int myspeed, int del) {

    Application_SmartRobotCarxxx0.Motion_Control = direction; //right
    ApplicationFunctionSet_SmartRobotCarMotionControl(Application_SmartRobotCarxxx0.Motion_Control /*direction*/, myspeed /*speed*/);
    if (del>0)
      delay(del);
}

void line_sensor_get(){

  left_line_sensor = analogRead(left_pin);
  right_line_sensor = analogRead(right_pin);

  LINE_SENSOR_CLEAR = ( (left_line_sensor<=offline) && (right_line_sensor<=offline) ); 
  TURN_LEFT = right_line_sensor <= offline && left_line_sensor > offline;
  TURN_RIGHT = left_line_sensor <= offline && right_line_sensor > offline;
}

void flame_sensor_get(){
  flame_sensor = analogRead(flame_pin);
}

void print_flame_sensor() {
    // Serial.print("Left: ");Serial.println(left_line_sensor); //left
    Serial.print("Flame Sensor: ");Serial.println(flame_sensor);//right
}

void print_line_sensor() {
    // Serial.print("Left: ");Serial.println(left_line_sensor); //left
    Serial.print("Right: ");Serial.println(right_line_sensor);//right
}

int ultrasonic_trig_echo(int TRIG_PIN, int ECHO_PIN){
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  return ((unsigned int)pulseIn(ECHO_PIN, HIGH) / 58);
}
void ultrasonic_get()
{
  // if (mode==2){
  //   tempda_3 = ultrasonic_trig_echo(TRIG_PIN_3, ECHO_PIN_3);
  //   tempda_4 = ultrasonic_trig_echo(TRIG_PIN_4, ECHO_PIN_4);
  //   tempda_5 = ultrasonic_trig_echo(TRIG_PIN_5, ECHO_PIN_5);
  // } else{
    // tempda_1 = ultrasonic_trig_echo(TRIG_PIN_1, ECHO_PIN_1);
    //tempda_2 = ultrasonic_trig_echo(TRIG_PIN_2, ECHO_PIN_2);
      // Servo1.write(75);
      // delay(100);
      // tempda_3 = ultrasonic_trig_echo(TRIG_PIN_4, ECHO_PIN_4);
      // Servo1.write(105);
      // delay(100);
    tempda_4 = ultrasonic_trig_echo(TRIG_PIN_4, ECHO_PIN_4);


    tempda_5 = ultrasonic_trig_echo(TRIG_PIN_5, ECHO_PIN_5);

    //IS_ON_TABLE = ( (tempda_1 <= max_distance) && (tempda_2 <= max_distance) );
    //ULTRASONIC_CLEAR = IS_ON_TABLE && FRONT_CLEAR;
  //}
  //REDUCE_SPEED = ( tempda_4 < obstacle_allowance && tempda_4 > stop_dist ) || ( tempda_3 < obstacle_allowance && tempda_3 > stop_dist );
  //FRONT_CLEAR = ( (tempda_3 > obstacle_allowance) && (tempda_4 > obstacle_allowance) );
}


void printUltraSonic() {
    // Serial.print("ULTRASONIC_1=");
    // Serial.print(tempda_1); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
    // Serial.println("cm   ");

    // Serial.print("ULTRASONIC_2=");
    // Serial.print(tempda_2); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
    // Serial.println("cm");

    // Serial.print("ULTRASONIC_3=");
    // Serial.print(tempda_3); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
    // Serial.println("cm   ");

    // Serial.print("ULTRASONIC_4=");
    // Serial.print(tempda_3); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
    // Serial.print("cm");
    // Serial.print(" ");
    // Serial.print(tempda_4);
    // Serial.println("cm");

    Serial.print("ULTRASONIC_5=");
    Serial.print(tempda_5); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
    Serial.println("cm");
}

