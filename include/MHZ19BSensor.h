/**
 * @file PMS7003Sensor.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MHZ19BSENSOR_H
#define MHZ19BSENSOR_H

#include <SoftwareSerial.h>
#include "esp_log.h"
#include <ErriezMHZ19B.h>
#include "configs.h"

#define MHZ19B_TX_PIN        33
#define MHZ19B_RX_PIN        25

// SoftwareSerial mhzSerial(MHZ19B_TX_PIN, MHZ19B_RX_PIN);
#define mhzSerial   Serial

ErriezMHZ19B mhz19b(&mhzSerial);

#define ERROR_MHZ_INIT_FAILED       0xe0
#define ERROR_MHZ_GET_DATA_FAILED   0xe1

/**
 * @brief Initialize ErriezMHZ19B Sensor
 *  
 * @return ERROR_CODE 
 * @retval ERROR_NONE: if success
 * @retval ERROR_PMS_INIT_FAILED: if failed
 */
ERROR_CODE mhz_init();

/**
 * @brief Get new data from ErriezMHZ19B sensor
 * 
 * @param[out] _co_2: PM CO2
 * 
 * @return ERROR_CODE 
 * @retval ERROR_NONE: if success
 * @retval ERROR_PMS_GET_DATA_FAILED: if failed
 */
ERROR_CODE mhz_getdata(uint32_t &_co_2);

#include "MHZ19BSensor.h"

ERROR_CODE mhz_init()
{
     char firmwareVersion[5];

    // Initialize senor software serial at fixed 9600 baudrate
    // mhzSerial.begin(9600);

    // Optional: Detect MH-Z19B sensor (check wiring / power)
    int mhz_decect_count = 0;
    while ( !mhz19b.detect()) {
        log_e("Detecting MH-Z19B sensor...");
        delay(2000);
        mhz_decect_count++;
        if(mhz_decect_count>10) return ERROR_MHZ_INIT_FAILED;
    };

    // Sensor requires 3 minutes warming-up after power-on
    while (mhz19b.isWarmingUp()) {
        log_e("Warming up...");
        delay(2000);
    };

    // Optional: Print firmware version
    // log_e("  Firmware: ");
    // mhz19b.getVersion(firmwareVersion, sizeof(firmwareVersion));
    // log_e("%s",firmwareVersion);

    // Optional: Set CO2 range 2000ppm or 5000ppm (default) once
    // log_e("Set range...");
    // mhz19b.setRange2000ppm();
    // mhz19b.setRange5000ppm();

    // Optional: Print operating range
    // log_e("  Range: ");
    // log_e("%d",mhz19b.getRange());
    // log_e("ppm");

    // Optional: Set automatic calibration on (true) or off (false) once
    // Serial.print(F("Set auto calibrate..."));
    // mhz19b.setAutoCalibration(true);

    // Optional: Print Automatic Baseline Calibration status
    // log_e("  Auto calibrate: ");
    // log_e("%s", mhz19b.getAutoCalibration() ? ("On") : ("Off"));

    return ERROR_NONE;
}

ERROR_CODE mhz_getdata(uint32_t &_co_2)
{
    int16_t result = 0;
    // Minimum interval between CO2 reads is required
    if (mhz19b.isReady()) {
        // Read CO2 concentration from sensor
        result  = mhz19b.readCO2();

        // Print result
        if (result < 0) {
            // An error occurred
            _co_2 = -1;
            log_e("MHZ19B Sensor get new data failed!");
            return ERROR_MHZ_GET_DATA_FAILED;
        } else {
            // Print CO2 concentration in ppm
            _co_2 = result;
            log_e("MHZ19B Sensor get new data successfully! %d",_co_2);
             return ERROR_NONE;
        }
    }
    return ERROR_MHZ_GET_DATA_FAILED;
}

#endif