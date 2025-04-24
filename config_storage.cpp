#include "melty_config.h"

#ifdef ENABLE_EEPROM_STORAGE

#include <Preferences.h>
Preferences preferences;

#define STORAGE_NAMESPACE "melty"
#define EEPROM_WRITTEN_SENTINEL_VALUE 42  // keep your value or define elsewhere

void save_settings_to_eeprom(int led_offset, float accel_radius, float accel_zero_g_offset) {
  preferences.begin(STORAGE_NAMESPACE, false); // RW mode

  preferences.putInt("sentinel1", EEPROM_WRITTEN_SENTINEL_VALUE);
  preferences.putInt("sentinel2", EEPROM_WRITTEN_SENTINEL_VALUE);
  
  preferences.putInt("led_offset", led_offset);
  preferences.putFloat("accel_radius", accel_radius);
  preferences.putFloat("zero_g_offset", accel_zero_g_offset);

  preferences.end();
}

bool check_sentinel() {
  preferences.begin(STORAGE_NAMESPACE, true); // RO mode

  int s1 = preferences.getInt("sentinel1", -1);
  int s2 = preferences.getInt("sentinel2", -1);

  preferences.end();

  return (s1 == EEPROM_WRITTEN_SENTINEL_VALUE && s2 == EEPROM_WRITTEN_SENTINEL_VALUE);
}

int load_heading_led_offset() {
  if (!check_sentinel()) return DEFAULT_LED_OFFSET_PERCENT;

  preferences.begin(STORAGE_NAMESPACE, true);
  int offset = preferences.getInt("led_offset", DEFAULT_LED_OFFSET_PERCENT);
  preferences.end();
  return offset;
}

float load_accel_zero_g_offset() {
  if (!check_sentinel()) return DEFAULT_ACCEL_ZERO_G_OFFSET;

  preferences.begin(STORAGE_NAMESPACE, true);
  float offset = preferences.getFloat("zero_g_offset", DEFAULT_ACCEL_ZERO_G_OFFSET);
  preferences.end();
  return offset;
}

float load_accel_mount_radius() {
  if (!check_sentinel()) return DEFAULT_ACCEL_MOUNT_RADIUS_CM;

  preferences.begin(STORAGE_NAMESPACE, true);
  float radius = preferences.getFloat("accel_radius", DEFAULT_ACCEL_MOUNT_RADIUS_CM);
  preferences.end();
  return radius;
}


void print_stored_config() {
  Serial.println("=== Stored Melty Config ===");

  if (!check_sentinel()) {
    Serial.println("No valid config found in EEPROM. Using defaults.");
    Serial.print("LED Offset (%): "); Serial.println(DEFAULT_LED_OFFSET_PERCENT);
    Serial.print("Accel Radius (cm): "); Serial.println(DEFAULT_ACCEL_MOUNT_RADIUS_CM, 3);
    Serial.print("Zero-G Offset: "); Serial.println(DEFAULT_ACCEL_ZERO_G_OFFSET, 3);
    return;
  }

  Serial.println("Valid config found:");
  Serial.print("LED Offset (%): "); Serial.println(load_heading_led_offset());
  Serial.print("Accel Radius (cm): "); Serial.println(load_accel_mount_radius(), 3);
  Serial.print("Zero-G Offset: "); Serial.println(load_accel_zero_g_offset(), 3);
}

#endif

////handles persistent storage of melty brain config (accelerometer radius, LED offset and accelerometer 0g offset)
////this code (unfortunately) depends on the AVR-specific EEPROM library:
////https://docs.arduino.cc/learn/built-in-libraries/eeprom
//
////if using on an ARM-based arduino - either just disable this (and hard code your settings in melty_config.h)
////...or you can try porting this functionality to ARM using the FlashStorage library:
////https://reference.arduino.cc/reference/en/libraries/flashstorage/
//
//#include "melty_config.h"
//
//#ifdef ENABLE_EEPROM_STORAGE
//
//#include <EEPROM.h>
//#include <arduino.h>
//#include "config_storage.h"
//
//#define EEPROM_WRITTEN_SENTINEL1_VALUE EEPROM_WRITTEN_SENTINEL_VALUE
//#define EEPROM_WRITTEN_SENTINEL2_VALUE EEPROM_WRITTEN_SENTINEL_VALUE
//
//#define EEPROM_WRITTEN_SENTINEL_BYTE1_LOC 0
//#define EEPROM_WRITTEN_SENTINEL_BYTE2_LOC 1
//
//#define EEPROM_HEADING_LED_LOC 2
//
////bytes for float
//#define EEPROM_ACCEL_RADIUS_BYTE1_LOC 3
//#define EEPROM_ACCEL_RADIUS_BYTE2_LOC 4
//#define EEPROM_ACCEL_RADIUS_BYTE3_LOC 5
//#define EEPROM_ACCEL_RADIUS_BYTE4_LOC 6
//
////bytes for float
//#define EEPROM_ACCEL_OFFSET_BYTE1_LOC 7
//#define EEPROM_ACCEL_OFFSET_BYTE2_LOC 8
//#define EEPROM_ACCEL_OFFSET_BYTE3_LOC 9
//#define EEPROM_ACCEL_OFFSET_BYTE4_LOC 10
//
//
////indicates values saved to EEPROM (doing 2x)
//static void write_sentinel() {
//  EEPROM.write(EEPROM_WRITTEN_SENTINEL_BYTE1_LOC, EEPROM_WRITTEN_SENTINEL1_VALUE);
//  EEPROM.write(EEPROM_WRITTEN_SENTINEL_BYTE2_LOC, EEPROM_WRITTEN_SENTINEL2_VALUE);
//}
//
////make sure we actually wrote values before trying to read... (doing 2x)
////if EEPROM_WRITTEN_SENTINEL_VALUE is changed - will cause EEPROM values to invalidate / use default values 
//static int check_sentinel() {
//  if (EEPROM.read(EEPROM_WRITTEN_SENTINEL_BYTE1_LOC) != EEPROM_WRITTEN_SENTINEL1_VALUE) return 0;
//  if (EEPROM.read(EEPROM_WRITTEN_SENTINEL_BYTE2_LOC) != EEPROM_WRITTEN_SENTINEL2_VALUE) return 0;
//  return 1;
//}
//
//void save_settings_to_eeprom(int led_offset, float accel_radius, float accel_zero_g_offset) {
//  EEPROM.write(EEPROM_HEADING_LED_LOC, led_offset);
//  EEPROM.put(EEPROM_ACCEL_RADIUS_BYTE1_LOC, accel_radius);
//  EEPROM.put(EEPROM_ACCEL_OFFSET_BYTE1_LOC, accel_zero_g_offset);
//  write_sentinel();
//}
//
//int load_heading_led_offset() {
//  //if value hasn't been saved previously - return the default
//  if (check_sentinel() != 1) return DEFAULT_LED_OFFSET_PERCENT;
//  return EEPROM.read(EEPROM_HEADING_LED_LOC);
//}
//
//float load_accel_zero_g_offset() {
//  //if value hasn't been saved previously - return the default
//  if (check_sentinel() != 1) return DEFAULT_ACCEL_ZERO_G_OFFSET;
//  float accel_zero_g_offset;
//  accel_zero_g_offset = EEPROM.get(EEPROM_ACCEL_OFFSET_BYTE1_LOC, accel_zero_g_offset);
//  return accel_zero_g_offset;
//}
//
//float load_accel_mount_radius() {
//  //if value hasn't been saved previously - return the default
//  if (check_sentinel() != 1) return DEFAULT_ACCEL_MOUNT_RADIUS_CM;
//  float accel_radius;
//  accel_radius = EEPROM.get(EEPROM_ACCEL_RADIUS_BYTE1_LOC, accel_radius);
//  return accel_radius;
//}
//
//#endif
//