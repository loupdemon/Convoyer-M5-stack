/**********************************************************************************************************************/
/* MAIN DEFINITIONS                                                                                                   */
/**********************************************************************************************************************/

#ifndef T_IOT_901_STG_1_MAIN_H
#define T_IOT_901_STG_1_MAIN_H

/**********************************************************************************************************************/
/* CORE HEADERS INCLUSIONS                                                                                            */
/**********************************************************************************************************************/

// Native
#include <iostream>
#include <string>

// Environment File
#include "env.local.h"

/**********************************************************************************************************************/
/* FRAMEWORK LIBRARIES                                                                                                 */
/**********************************************************************************************************************/

// Framework main header
#include <Arduino.h>

// Time, BUS (I2C, GPIO...) and SPI interface
#include <ctime>
#include <Wire.h>
#include <SPI.h>

// Wi-Fi
#include "WiFi.h"
#include "WiFiMulti.h"

// Request HTTP
#include "HTTPClient.h"
#include "WiFiClient.h"

// NVS
#include "Preferences.h"

/**********************************************************************************************************************/
/* PIO DEPENDENCIES                                                                                                   */
/**********************************************************************************************************************/

// NFC, M5Stack core, Stepper and JSON
//#include "MFRC522.h"
#include "M5Stack.h"
#include "Stepper.h"
//#include "ArduinoJson.h"

/**********************************************************************************************************************/
/* COMPONENTS HEADERS                                                                                                 */
/**********************************************************************************************************************/

// API handler, StepMotor and ServoMotor
#include "Request.h"
#include "StepMotor.h"
#include "ServoMotor.h"


/**********************************************************************************************************************/
/* PRIVATE LIBRARIES HEADERS                                                                                          */
/**********************************************************************************************************************/

// RFID, GRBL, GOPLUS ADN M5EZ
#include "MFRC522_I2C.h"
#include "MODULE_GRBL13.2.h"
#include "GoPlus2.h"
#include "Arduino_JSON.h"
#include <cassert>

/**********************************************************************************************************************/
/* USEFUL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

// Stepper Motor
#define STEPMOTOR_I2C_ADDR 0x70

// NFC Reader
#define SS_PIN 10
#define RST_PIN 5
#define RELAY_PIN A5 // the Arduino pin connects to relay

// JSON
#define JSON_DOC_SIZE 4096

// General
#define DELAY 1000
#define USE_SERIAL Serial
#define SERIAL_TYPE 115200

// EXITS
#define LEFT_EXIT 1099
#define MIDDLE_EXIT 900
#define RIGHT_EXIT 800

using namespace std;

#endif //T_IOT_901_STG_1_MAIN_H




/**********************************************************************************************************************/
/* END OF FILE                                                                                                        */
/**********************************************************************************************************************/
