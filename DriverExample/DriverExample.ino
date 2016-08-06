/**
  ******************************************************************************
  * @file    car_driver_shield_sample0.cpp
  * @author  Elechouse Team
  * @version V1.0
  * @date    2013-05-16
  * @brief   This file provides a demostration on 
              how to use CarDriverShield driver
  ******************************************************************************
  * @note:
         This sample supports elechouse car driver shield. Usage:
         w --> forward; s --> backward; a --> turn left; d --> turn right;
         e --> accelerate; c --> decelerate; z --> stop;
         This demo also can be used with Openwrt(wifi car project).
  ******************************************************************************
  * @section  HISTORY
  
    V1.0    Initial version.
  */

#include "CarDriverShield.h"

/** declare led pin number*/
int led = 13;
//int led = -32768; // don't use led

/** declare car driver class */
CarDriverShield myCar;
bool commonLvl = HIGH;

void setup()
{
  Serial.begin(9600);

  Serial.println("-----------Smart Car --------------\r\n");

  /** car initial */
//  myCar.begin(led);    // specify a LED port, any pin except PWM pins. 
//  myCar.begin();  // don't need LED indicate

  pinMode(MOTOR_B_NPORT, OUTPUT);
  pinMode(MOTOR_B_PPORT, OUTPUT);

  digitalWrite(MOTOR_B_NPORT, commonLvl);
  digitalWrite(MOTOR_B_PPORT, commonLvl);


}

bool start = false;

void loop()
{
  uint8_t buf;
  /** envent loop function, must be polled in loop() */
//  myCar.evt_loop();
  int pot = analogRead( A0 ) >> 2;
  
  if (start) {
    analogWrite( MOTOR_B_PPORT, pot );
  }
  
  if(Serial.available()){
    buf=Serial.read();
    Serial.write(buf);

    switch(buf){
    case 'w': 
    case 'W':
      /** forward */
      start = true;
      Serial.println(pot);
//      myCar.forward();
      break;
    case 's': 
    case 'S':
      /** backward */
//      myCar.backward();
      break;
    case 'z': 
    case 'Z':
      /** stop */
      start = false;
      digitalWrite(MOTOR_B_PPORT, commonLvl);
//      myCar.stop();
      break;
    case 'e': 
    case 'E':
      /** accelerate */
//      myCar.accelerate();
      break;
    case 'c': 
    case 'C':
      /** decelerate */
//      myCar.decelerate();
      break;
    case 'a': 
    case 'A':
      /** turn left*/
//      myCar.turn_left();
      break;
    case 'd': 
    case 'D':
      /** turn right*/
//      myCar.turn_right();
      break;
    }
  }
}

