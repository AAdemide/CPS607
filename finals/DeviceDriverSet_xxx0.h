/*
 * @Author: ELEGOO
 * @Date: 2019-10-22 11:59:09
 * @LastEditTime: 2020-06-12 14:45:27
 * @LastEditors: Changhua
 * @Description: conqueror robot tank
 * @FilePath: 
 */
#ifndef _DeviceDriverSet_xxx0_H_
#define _DeviceDriverSet_xxx0_H_
#include <Arduino.h>

/*ITR20001 Detection*/
class DeviceDriverSet_ITR20001
{
public:
  bool DeviceDriverSet_ITR20001_Init(void);
  float DeviceDriverSet_ITR20001_getAnaloguexxx_L(void);
  float DeviceDriverSet_ITR20001_getAnaloguexxx_M(void);
  float DeviceDriverSet_ITR20001_getAnaloguexxx_R(void);
#if _Test_DeviceDriverSet
  void DeviceDriverSet_ITR20001_Test(void);
#endif

private:
//03
// #define PIN_ITR20001xxxL A0
// #define PIN_ITR20001xxxM A1
// #define PIN_ITR20001xxxR A2
//04
#define PIN_ITR20001xxxL A5
#define PIN_ITR20001xxxM A4
#define PIN_ITR20001xxxR A3


#define left_pin A3
// #define middle_pin A4
#define right_pin A5
};


/*ULTRASONIC*/

//#include <NewPing.h>
class DeviceDriverSet_ULTRASONIC
{
public:
  void DeviceDriverSet_ULTRASONIC_Init(void);
  void DeviceDriverSet_ULTRASONIC_Test(void);
  void DeviceDriverSet_ULTRASONIC_Get(uint16_t *ULTRASONIC_Get_1, uint16_t *ULTRASONIC_Get_2 /*out*/);

private:
#define TRIG_PIN_1 13      // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_1 12     // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define TRIG_PIN_2 10      // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_2 11      // Arduino pin tied to echo pin on the ultrasonic sensor.

#define TRIG_PIN_3 4      // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_3 9      // Arduino pin tied to echo pin on the ultrasonic sensor.

#define TRIG_PIN_4 A0      // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_4 A1     // Arduino pin tied to echo pin on the ultrasonic sensor.

#define TRIG_PIN_5 2      // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_5 A2     // Arduino pin tied to echo pin on the ultrasonic sensor.

};

/*Motor*/
class DeviceDriverSet_Motor
{
public:
  void DeviceDriverSet_Motor_Init(void);
#if _Test_DeviceDriverSet
  void DeviceDriverSet_Motor_Test(void);
#endif
  void DeviceDriverSet_Motor_control(boolean direction_A, uint8_t speed_A, //A组电机参数
                                     boolean direction_B, uint8_t speed_B, //B组电机参数
                                     boolean controlED                     //AB使能允许 true
  );                                                                       //电机控制
private:

#define PIN_Motor_PWMA 5
#define PIN_Motor_PWMB 6
#define PIN_Motor_BIN_1 8
#define PIN_Motor_AIN_1 7
#define PIN_Motor_STBY 3

public:
#define speed_Max 255
#define direction_just true
#define direction_back false
#define direction_void 3

#define Duration_enable true
#define Duration_disable false
#define control_enable true
#define control_disable false
};

#endif
