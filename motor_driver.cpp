//this module handles interfacing to the motors

#include "arduino.h"
#include "melty_config.h"
#include "motor_driver.h"
#include <ESP32Servo.h>
Servo motor;
Servo motor1;

//motor_X_on functions are used for the powered phase of each rotation
//motor_X_coast functions are used for the unpowered phase of each rotation
//motor_X_off functions are used for when the robot is spun-down

void motor_on(float throttle_percent, int motor_pin) {

  if (THROTTLE_TYPE == FIXED_PWM_THROTTLE) {
    if(motor_pin == MOTOR_PIN1){
      motor.write(PWM_MOTOR_ON);
    } 
    if(motor_pin == MOTOR_PIN2) {
      motor1.write(PWM_MOTOR_ON);
    }
  }

  if (THROTTLE_TYPE == BINARY_THROTTLE) {
    digitalWrite(motor_pin, HIGH);
  }

//If DYNAMIC_PWM_THROTTLE - PWM is scaled between PWM_MOTOR_COAST and PWM_MOTOR_ON
//Applies over range defined by DYNAMIC_PWM_THROTTLE_PERCENT_MAX - maxed at PWM_MOTOR_ON above this
  if (THROTTLE_TYPE == DYNAMIC_PWM_THROTTLE) {
    float throttle_pwm = PWM_MOTOR_COAST + ((throttle_percent / DYNAMIC_PWM_THROTTLE_PERCENT_MAX) * (PWM_MOTOR_ON - PWM_MOTOR_COAST));
    if (throttle_pwm > PWM_MOTOR_ON) throttle_pwm = PWM_MOTOR_ON;
    
    if(motor_pin == MOTOR_PIN1){
      motor.write(throttle_pwm);
    } 
    if(motor_pin == MOTOR_PIN2) {
      motor1.write(throttle_pwm);
    }
  }
}

void motor_1_on(float throttle_percent) {
  motor_on(throttle_percent, MOTOR_PIN1);
}

void motor_2_on(float throttle_percent) {
  motor_on(throttle_percent, MOTOR_PIN2);
}

void motor_coast(int motor_pin) {
  if (THROTTLE_TYPE == FIXED_PWM_THROTTLE || THROTTLE_TYPE == DYNAMIC_PWM_THROTTLE) {
    if(motor_pin == MOTOR_PIN1){
      motor.write(PWM_MOTOR_COAST);
    } 
    if(motor_pin == MOTOR_PIN2) {
      motor1.write(PWM_MOTOR_COAST);
    }
  }
  if (THROTTLE_TYPE == BINARY_THROTTLE) {
    digitalWrite(motor_pin, LOW);  //same as "off" for brushed motors
  }
}

void motor_1_coast() {
  motor_coast(MOTOR_PIN1);
}

void motor_2_coast() {
  motor_coast(MOTOR_PIN2);
}

void motor_off(int motor_pin) {
  if (THROTTLE_TYPE == FIXED_PWM_THROTTLE || THROTTLE_TYPE == DYNAMIC_PWM_THROTTLE) {
    if(motor_pin == MOTOR_PIN1){
      motor.write(PWM_MOTOR_OFF);
    } 
    if(motor_pin == MOTOR_PIN2) {
      motor1.write(PWM_MOTOR_OFF);
    }
  }
  if (THROTTLE_TYPE == BINARY_THROTTLE) {
    digitalWrite(motor_pin, LOW);  //same as "off" for brushed motors
  }
}

void motor_1_off() {
  motor_off(MOTOR_PIN1);
}

void motor_2_off() {
  motor_off(MOTOR_PIN2);
}

void motors_off() {
  motor_1_off();
  motor_2_off();
}

void init_motors() {
  motor.setPeriodHertz(100);    // standard 50 hz servo
	motor.attach(MOTOR_PIN1, 1000, 2000);
  motor1.setPeriodHertz(100);    // standard 50 hz servo
	motor1.attach(MOTOR_PIN2, 1000, 2000);

  motor.write(PWM_MOTOR_COAST);
  motor1.write(PWM_MOTOR_COAST);
  delay(2000);
  //pinMode(MOTOR_PIN1, OUTPUT);
  //pinMode(MOTOR_PIN2, OUTPUT);
  motors_off();
}
