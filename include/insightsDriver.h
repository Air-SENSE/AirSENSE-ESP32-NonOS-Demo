/**
 * @file insightsDriver.h
 * @author SPARC member
 * @brief ESP Insights driver library
 * @version 0.1
 * @date 2023-01-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef INSIGHtSDRIVER_H
#define INSIGHtSDRIVER_H

#include <Arduino.h>
#include "esp32-hal-log.h"
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "configs.h"
#include <esp_insights.h>
#include "esp_diagnostics_system_metrics.h"
#include <esp_diagnostics_metrics.h>
#include <esp_diagnostics_variables.h>
#include "esp_rmaker_utils.h"
#include "esp_log.h"
#include <WiFi.h>


#define ESP_INSIGHTS_AUTH_KEY "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VyIjoiMjUyY2FlNWYtMThmMy00YmM4LThmZTEtMGQ5MWUwMzMwZTA4IiwiZXhwIjoxOTg5MDY3NTk3LCJpYXQiOjE2NzM3MDc1OTcsImlzcyI6ImUzMjJiNTljLTYzY2MtNGU0MC04ZWEyLTRlNzc2NjU0NWNjYSIsInN1YiI6IjdhYTlkMmNkLTVhYTItNGU3Yy05ZTY3LWZhNDJmODgyMzYwYyJ9.vyRc2fueIS0JjP6apXg4__bf7wwPGkxjz7KjMwkzf-Y2mKjqfmYJCfSiRwzyRU9Y767ZOzinKvmkqF0DGvpqoAIyLCudi5fylnua6mZcNmxeTFL4jMIMbmYXQOnEBul-qpZCMM_1fkPh2z0_Hi7VRfksEjyyfc_7hpeINfQba1vowc1lL6JxDLmdYP4rs60Y4VQLKyGFMUxkazwV_HREtvgxsBU05IS6LDxLJuUTR_LpPnON0qy9XYHR9EXRkgH8qejbOtId1GBrmDnwTVL5eF7UtkOVvvXTW9NqNMAWJ9yctRCLyFPllFJNrvuZXTvCa6uXe74HBG6qBp-h7QkNbg"
#define METRICS_DUMP_INTERVAL           20 * 1000
#define METRICS_DUMP_INTERVAL_TICKS     (METRICS_DUMP_INTERVAL / portTICK_RATE_MS)
static const char *TAG_INSIGHTS = "INSIGHTS";

unsigned long lastPublishedHeadMetrics = millis();
unsigned long lastPublishedWifiMetrics = millis();
bool insightsEnabled = true;
bool insightsLoop = true;

#define ERROR_INSIGHTS_INT_FAILED       0xe0
#define ERROR_INSIGHTS_POST_DATA_FAILED   0xe1

#include <firmware_info.h>

/**
 * @brief Initialize insights
 *  
 * @return ERROR_CODE 
 * @retval ERROR_NONE: if success
 * @retval ERROR_PMS_INIT_FAILED: if failed
 */
ERROR_CODE insights_init();

/**
 * @brief Get new data from ErriezMHZ14A sensor
 * 
 * @param[out] _co_2: PM CO2
 * 
 * @return ERROR_CODE 
 * @retval ERROR_NONE: if success
 * @retval ERROR_PMS_GET_DATA_FAILED: if failed
 */
ERROR_CODE insights_post_data();

void init_insights(void){
    esp_rmaker_time_sync_init(NULL);
    esp_insights_config_t config = {
      .log_type = ESP_DIAG_LOG_TYPE_ERROR | ESP_DIAG_LOG_TYPE_WARNING | ESP_DIAG_LOG_TYPE_EVENT,
      .auth_key = ESP_INSIGHTS_AUTH_KEY,
    };

    esp_err_t ret = esp_insights_init(&config);
    if (ret != ESP_OK) {
        log_e( "Failed to initialize ESP Insights, err:0x%x", ret);
    }
    // ESP_ERROR_CHECK(ret);

    esp_diag_heap_metrics_dump();
    esp_diag_wifi_metrics_dump();

    // Register a Metric:
    esp_diag_metrics_register("ControlDeviceStatus", "CDstatus", "ControlDevice current status", "AccessCtrl", ESP_DIAG_DATA_TYPE_UINT);
    // uint32_t statusNum = 12;
    // esp_diag_metrics_add_uint("CDstatus", statusNum);

    esp_diag_metrics_register("ControlDevice button count", "pushCount", "ControlDevice button push count", "AccessCtrl", ESP_DIAG_DATA_TYPE_UINT);
    

    // Register a Variable:
    esp_diag_variable_register("Wi-Fi", "lastEvent", "Last event", "Wi-Fi.Events", ESP_DIAG_DATA_TYPE_UINT);
    esp_diag_variable_add_uint("lastEvent", 7);
    
    esp_diag_variable_register("AccessCtrl", "lockStatus", "Lock status", "AccessCtrl.ControlDevice", ESP_DIAG_DATA_TYPE_STR);


}



static const char *insightsTAG = "esp_insights";

unsigned long currentLoopMillis = 0;
unsigned long previousMainLoopMillis = 0;

ERROR_CODE insights_init() {

    // Open USB serial port
    // Serial.begin(115200);
    // Serial.setDebugOutput(true);
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    esp_log_level_set("*", ESP_LOG_ERROR);
    esp_log_level_set("cpu_start", ESP_LOG_INFO);
    esp_log_level_set(insightsTAG, ESP_LOG_VERBOSE);
    esp_log_level_set("esp_core_dump_elf", ESP_LOG_VERBOSE);

    // show_flash_info();
    // show_firmware_description();


    while (WiFi.status() != WL_CONNECTED) {
        return ERROR_INSIGHTS_INT_FAILED;
    }
    init_insights();

    log_i("###  Looping time");

    return ERROR_NONE;
}


ERROR_CODE insights_post_data() 
{

    currentLoopMillis = millis();

    if(insightsEnabled && insightsLoop && (currentLoopMillis-lastPublishedHeadMetrics > METRICS_DUMP_INTERVAL)){
        lastPublishedHeadMetrics = currentLoopMillis;
        esp_diag_heap_metrics_dump();
        log_d("ESP-Insights heap metrics updated from loop");
        return ERROR_NONE;
    }
    if(insightsEnabled && insightsLoop && (currentLoopMillis-lastPublishedWifiMetrics > METRICS_DUMP_INTERVAL + METRICS_DUMP_INTERVAL / 2)){
        lastPublishedWifiMetrics = currentLoopMillis;
        esp_diag_wifi_metrics_dump();
        log_d("ESP-Insights  wifi metrics updated from loop");
        return ERROR_NONE;
    }
    return ERROR_INSIGHTS_POST_DATA_FAILED;
}

#endif