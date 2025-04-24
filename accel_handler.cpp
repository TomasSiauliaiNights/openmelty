//this module interfaces with the accelerometer to provide current G-force level

//Two breakout boards using the H3LIS331DL accelerometer have been tested / verified to work

//Sparkfun's H3LIS331DL breakout: https://www.sparkfun.com/products/14480 (3v part)
//Requires using  to interface with 5v arduino

//Adafruit's HSLIS311 breakout includes a level converter - is a single component solution:
//https://www.adafruit.com/product/4627

//Update ACCEL_I2C_ADDRESS in accel_handler.h with I2c address (Adafruit and Sparkfun boards are different!)

//Sparkfun's LIS311 library is used (also works for Adafruit part)

#include <arduino.h>
#include "melty_config.h"
#include "accel_handler.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL375.h>

#define I2C_SDA 8
#define I2C_SCL 9

Adafruit_ADXL375 accel = Adafruit_ADXL375(12345);

void init_accel() {

  //Wire.begin();
  
    //increase I2C speed to reduce read times a bit
                          //value of 400000 allows accel read in ~1ms and
                          //is verfied to work with Sparkfun level converter
                          //(some level converters have issues at higher speeds)
  
  Wire.begin(I2C_SDA, I2C_SCL);
  //Wire.setClock(400000);
  //while (!Serial);
  Serial.println("ADXL375 Accelerometer Test"); Serial.println("");

  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL375 ... check your connections */
    Serial.println("Ooops, no ADXL375 detected ... Check your wiring!");
    while(1);
  }

  //sets accelerometer to specified scale (100, 200, 400g)
  //xl.setFullScale(ACCEL_RANGE);
}

//reads accel and converts to G's
//ACCEL_MAX_SCALE needs to match ACCEL_RANGE value
float get_accel_force_g() {
  //int16_t x, y, z;
  //xl.readAxes(x, y, z);
  //event.acceleration.z
  //return xl.convertToG(ACCEL_MAX_SCALE,x);
  float g_value;
  sensors_event_t event;
  accel.getEvent(&event);
  g_value = event.acceleration.y / 9.81;
  return g_value;
}
