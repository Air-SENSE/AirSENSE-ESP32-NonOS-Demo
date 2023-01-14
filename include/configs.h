/*
 * File: configs.h
 * Authors: Hank <lctuyenvn@gmail.com>
 * Create: 2019-04-17 16:51:03
 *
 * Copyright (c) 2019 EmBCI. All right reserved.
 *
 * Modify this file to quickly configure ESP32_Sandbox,
 * which is the firmware of ESP32 used in project EmBCI.
 */

#ifndef CONFIGS_H
#define CONFIGS_H

#define NAME_DEVICE "SPARC-1"

#define SERIAL_DEBUG_BAUDRATE   (9600)
#define DEVICE_DATA_SAVE_INTERVAL 5000
#define ERROR_CODE int
#define ERROR_NONE (0x00)

enum status_et
{
    DISCONNECTED,
    CONNECTED,
    CONNECTION_LOST,
    SCANNING,
    SENDING_DATA,
    READING_DATA,
    WRITING_DATA
};

struct connectionStatus
{
    status_et wifiStatus;
    status_et sdCardStatus;
    status_et mqttConnection;
    status_et ds3231Module;
    status_et bmeSensor;
    status_et pmsSensor;
    status_et mhzSensor;
};

//========================== DEFINE FOR WIFI ========================

#define USING_WIFI_CONFIG
#define WIFI_CONNECT_INTERVAL 30000
#define WIFI_MAX_CONNECT_TRIAL 50
const char ssid[] = "BK Star";
const char pass[]  = "bkstar2021";

//========================== DEFINE FOR LOGGING ========================

#define USING_SDCARD
#define USING_MQTT
#define USING_RTC

//========================== DEFINE SPI  ========================

#define PIN_NUM_MISO    (21)
#define PIN_NUM_MOSI    (19)
#define PIN_NUM_CLK     (18)
#define PIN_CS_SD_CARD  (5)

//========================== DEFINE I2C ========================

#define PIN_SDA_GPIO    26
#define PIN_SCL_GPIO    27
#define I2C_CLOCK_SPEED 100000U
#define RTC_I2C_ADDRESS (0x68)

//========================== DEFINE LED RGB ========================

#define PIN_NEO_PIXEL   (23)

//========================== DEFINE 2 BUTTON ========================

#define PIN_BUTTON_1    (35)
#define PIN_BUTTON_2    (34)


#endif // CONFIGS_H