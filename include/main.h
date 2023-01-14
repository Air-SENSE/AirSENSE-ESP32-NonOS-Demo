#ifndef _MAIN_H_
#define _MAIN_H_

// Arduino core @ ${ARDUINO_ESP32}/cores/esp32
#include <Arduino.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ArduinoJson.h>


// local headers
#include "configs.h"
// #include "globals.h"

// #include "DeviceManager.h"
#include "DataStructure.h"
#include "WiFiManager.h"
#include "BMESensor.h"
#include "PMS7003Sensor.h"
#include "MHZ14ASensor.h"
#include "DS3231Driver.h"
#include "MQTTConnection.h"



/******************************************************************************
 * Global variables definition
 */

const char *NAME = "AirSENSE";
const char *prompt = "AirSENSE> ";

char dateTime_string[25];
char dataCalib_string[50];
char sensorDataString[64];
String messageData;

struct sensorData sensorData_st;
struct calibData calibData_st;
struct connectionStatus connectionStatus_st;


char espID[10];
char topic[32];

WiFiUDP ntpUDP;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
NTPClient timeClient(ntpUDP, "de.pool.ntp.org", 3600*7, 60000);

const char fileNameCalib[] = "calib.txt";			// file chua cac gia tri calib
char nameFileSaveData[12];							// ten file luu du lieu cua sensor theo tung ngay

const char* remote_host_string = "www.google.com";

RTC_DS3231 realTime;


#endif