/**
 * @file BMESensor.h
 * @author SPARC member
 * @brief BME280 Sensor library
 * @version 0.1
 * @date 2022-11-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef BMESENSOR_H
#define BMESENSOR_H

#include <Wire.h>
#include "esp_log.h"
#include "BME280I2C.h"
#include "configs.h"

#define ERROR_BME_INIT_FAILED       0xd0
#define ERROR_BME_DET_DATA_FAILED   0xd1

BME280I2C bmeSensor;

/**
 * @brief Initialize BME280 Sensor
 * 
 * @param[in] _wire: Wire I2C
 * 
 * @return ERROR_CODE 
 * @retval ERROR_NONE: if success
 * @retval ERROR_BME_INIT_FAILED: if failed
 */
ERROR_CODE bme_initialize(TwoWire &_wire);


/**
 * @brief Get temperature, humidity and presure value from BME280 Sensor 
 * 
 * @param[out] _temperature: temperature value (℃)
 * @param[out] _humidity: humidity value (%)
 * @param[out] _pressure: pressure value (atm)
 * 
 * @return ERROR_CODE
 * @retval ERROR_NONE: if success
 * @retval ERROR_BME_DET_DATA_FAILED: if failed
 */
ERROR_CODE bme_readData(float &_temperature, float &_humidity, float &_pressure);

ERROR_CODE bme_initialize(TwoWire &_wire)
{
    // Wire.begin(PIN_SDA_GPIO, PIN_SCL_GPIO, I2C_CLOCK_SPEED);

    if (bmeSensor.begin())
    {
        log_e("BME280 Sensor initialize successfully!");
        // connectionStatus_st.bmeSensor = status_et::CONNECTED;
        return ERROR_NONE;
    } else {
        log_e("BME280 Sensor initialize failed!");
        // connectionStatus_st.bmeSensor = status_et::DISCONNECTED;
        return ERROR_BME_INIT_FAILED;
    }

};


ERROR_CODE bme_readData(float &_temperature, float &_humidity, float &_pressure)
{
    bmeSensor.read(_pressure, _temperature, _humidity, BME280::TempUnit::TempUnit_Celsius, BME280::PresUnit::PresUnit_atm);
    _pressure = (_pressure == NAN) ? -1 : _pressure;
    _temperature = (_temperature == NAN) ? -273 : _temperature;
    _humidity = (_humidity == NAN) ? -1 : _humidity;
    if (_pressure == -1 || _temperature == -273 || _humidity == -1)
    {
        log_e("BME280 Sensor get data failed!");
        return ERROR_BME_DET_DATA_FAILED;
    } else {
        log_i("BME280 Sensor get data successsfully!");
        return ERROR_NONE;
    }
};

#endif