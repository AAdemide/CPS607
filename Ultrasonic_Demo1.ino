/*
 * @Author: ELEGOO
 * @Date: 2019-10-22 11:59:09
 * @LastEditTime: 2020-06-28 14:55:26
 * @LastEditors: Changhua
 * @Description: conqueror robot tank
 * @FilePath: 
 */
#include <avr/wdt.h>
#include "DeviceDriverSet_xxx0.h"
#include "ApplicationFunctionSet_xxx0.cpp"

DeviceDriverSet_ULTRASONIC myUltrasonic;
DeviceDriverSet_Motor AppMotor;
Application_xxx Application_SmartRobotCarxxx0;
uint16_t *ULTRASONIC_Get_1; uint16_t *ULTRASONIC_Get_2;
unsigned int tempda_1 = 0;
unsigned int tempda_2 = 0;
int speed=70;
int max_distance=10;

void setup()
{
  Serial.begin(9600);
  myUltrasonic.DeviceDriverSet_ULTRASONIC_Init();
  AppMotor.DeviceDriverSet_Motor_Init();
  delay(2000);
  Application_SmartRobotCarxxx0.Motion_Control = Left;
  ApplicationFunctionSet_SmartRobotCarMotionControl(Application_SmartRobotCarxxx0.Motion_Control /*direction*/, speed /*speed*/);
  delay(500);
  
}

void loop()
{

DeviceDriverSet_ULTRASONIC_Get();
printUltraSonic(tempda_1, tempda_2);

// ------ if sensor is on the table move forward
if((tempda_1 <= max_distance) && (tempda_2 <= max_distance)){
  Application_SmartRobotCarxxx0.Motion_Control = Forward;
  ApplicationFunctionSet_SmartRobotCarMotionControl(Application_SmartRobotCarxxx0.Motion_Control /*direction*/, speed+10 /*speed*/);
  // delay(500);
}else {// ------ if sensor is off the table enter else branch
  Application_SmartRobotCarxxx0.Motion_Control = stop_it; //stop
  ApplicationFunctionSet_SmartRobotCarMotionControl(Application_SmartRobotCarxxx0.Motion_Control /*direction*/, speed /*speed*/);
  // delay(500);
  Application_SmartRobotCarxxx0.Motion_Control = Backward; //stop
  ApplicationFunctionSet_SmartRobotCarMotionControl(Application_SmartRobotCarxxx0.Motion_Control /*direction*/, speed /*speed*/);
  delay(200);
   // if sensor is off the table turn until one of the sensors is on the table (should quit)
  // while((*ULTRASONIC_Get_1 > max_distance) || (*ULTRASONIC_Get_2 <= max_distance)){
    if ((tempda_1 <= max_distance)){ //if one
      // Serial.print("Two");
        Application_SmartRobotCarxxx0.Motion_Control = Right; //right
        ApplicationFunctionSet_SmartRobotCarMotionControl(Application_SmartRobotCarxxx0.Motion_Control /*direction*/, speed+5 /*speed*/);
        delay(1000);
    }else{
      // Serial.print("One");
      Application_SmartRobotCarxxx0.Motion_Control = Left; //left
      ApplicationFunctionSet_SmartRobotCarMotionControl(Application_SmartRobotCarxxx0.Motion_Control /*direction*/, speed+5 /*speed*/);
      delay(1000);
    }
    
    DeviceDriverSet_ULTRASONIC_Get();
    printUltraSonic(tempda_1, tempda_2);

  }


}

void printUltraSonic(int one, int two) {
    Serial.print("ULTRASONIC_1=");
    Serial.print(one); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
    Serial.print("cm   ");

    Serial.print("ULTRASONIC_2=");
    Serial.print(two); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
    Serial.println("cm");
}

void DeviceDriverSet_ULTRASONIC_Get(void)
{
  digitalWrite(TRIG_PIN_1, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_1, LOW);
  tempda_1 = ((unsigned int)pulseIn(ECHO_PIN_1, HIGH) / 58);

  digitalWrite(TRIG_PIN_2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_2, LOW); 
  tempda_2 = ((unsigned int)pulseIn(ECHO_PIN_2, HIGH) / 58);

}



