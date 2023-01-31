/**
 * @file MHZ14ASensor.h
 * @author lctuyenvn@gmail.com
 * @brief hocj2me
 * @version 0.1
 * @date 2023-01-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MHZ14ASENSOR_H
#define MHZ14ASENSOR_H

#include <SoftwareSerial.h>
#include "esp_log.h"
#include "configs.h"

#define MHZ14A_PWM_PIN        23


#define ERROR_MHZ_INIT_FAILED       0xe0
#define ERROR_MHZ_GET_DATA_FAILED   0xe1

enum MHZ_DATA_RANGES {RANGE_2K = 2000, RANGE_5K = 5000};

MHZ_DATA_RANGES mhz_Ranges = RANGE_5K;


unsigned long pulseInTimeBegin = millis();
unsigned long pulseInTimeEnd = millis();
bool is_new_mhz_data = false;

void buttonPinInterrupt()
{
  if (digitalRead(MHZ14A_PWM_PIN) == HIGH) {
    // start measuring
    pulseInTimeBegin = millis();
  }
  else {
    // stop measuring
    pulseInTimeEnd = millis();
    is_new_mhz_data = true;
  }
}

/**
 * @brief Initialize ErriezMHZ14A Sensor
 *  
 * @return ERROR_CODE 
 * @retval ERROR_NONE: if success
 * @retval ERROR_PMS_INIT_FAILED: if failed
 */
ERROR_CODE mhz_init();

/**
 * @brief Get new data from ErriezMHZ14A sensor
 * 
 * @param[out] _co_2: PM CO2
 * 
 * @return ERROR_CODE 
 * @retval ERROR_NONE: if success
 * @retval ERROR_PMS_GET_DATA_FAILED: if failed
 */
ERROR_CODE mhz_getdata(uint32_t &_co_2);


ERROR_CODE mhz_init()
{
    pinMode(MHZ14A_PWM_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(MHZ14A_PWM_PIN),
                  buttonPinInterrupt,
                  CHANGE);
    long t0 = millis();
    while (millis() - t0 < 5 * 1000) 
    {
        log_i("Warming up...");
        delay(1000);
    }
    return ERROR_NONE;
}

ERROR_CODE mhz_getdata(uint32_t &_co_2)
{
    unsigned long time_low, time_high, ppm_pwm;
    if (is_new_mhz_data) 
    {
        is_new_mhz_data = false;
        time_high = pulseInTimeEnd - pulseInTimeBegin;
        time_low = 1004 - time_high;
        ppm_pwm = mhz_Ranges * (time_high - 2) / (time_high + time_low - 4);
        _co_2 = ppm_pwm;
        log_i("MHZ14A Sensor get new data successfully! %d",_co_2);
        return ERROR_NONE;
    }
    else
    {
        _co_2 = -1;
        return ERROR_MHZ_GET_DATA_FAILED;
    }

}

#endif