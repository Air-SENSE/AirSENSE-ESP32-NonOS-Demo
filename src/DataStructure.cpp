#include "DataStructure.h"

ERROR_CODE creatCalibDataString(char *_calibDataString, struct calibData _calibData)
{
	sprintf(_calibDataString, "%u|%u|%u|%u|%u|%u|%u\n",
							  _calibData.temperature_calibInt_u32,
							  _calibData.humidity_calibInt_u32,
							  _calibData.pm1_calibInt_u32,
							  _calibData.pm10_calibInt_u32,
							  _calibData.pm25_calibInt_u32,
							  _calibData.temperature_calibFloat_u32,
							  _calibData.humidity_calibFloat_u32 );
	if (_calibDataString[0] == '\0')
	{
		ESP_LOGI("Create string failed!");
		return ERROR_DATA_CREATE_STRINGDATA_FAILED;
	} else {    
        ESP_LOGI("%s",_calibDataString);

		ESP_LOGI("Create string complete!");
		return ERROR_NONE;
	}
}


ERROR_CODE createSensorDataString(char *_sensorDataString,
                                  const char *_nameDevice,
                                  const char *_dateTimeString,
                                  struct sensorData _sensorData_st)
{
    strcpy(_sensorDataString, "");
    sprintf(_sensorDataString, "%s,%s,%.1f,%.1f,%f,%u,%u,%u,%u,",
                                _nameDevice,
                                _dateTimeString,
                                _sensorData_st.temperature,
                                _sensorData_st.humidity,
                                _sensorData_st.pressure_u32,
                                _sensorData_st.pm1_u32,
                                _sensorData_st.pm25_u32,
                                _sensorData_st.pm10_u32,
                                _sensorData_st.co_2_u32);
    
	if (_sensorDataString[0] == '\0')
	{
		ESP_LOGI("Create string failed!");
		return ERROR_DATA_CREATE_STRINGDATA_FAILED;
	} else {
		ESP_LOGI("Create string complete!");
		return ERROR_NONE;
	}
}

ERROR_CODE createMessageMQTTString( String &messageMQTTString,
                                    const char *_espID,
                                    NTPClient& _timeClient,
                                    struct sensorData _sensorData_st)
{
	_timeClient.update();

    StaticJsonDocument<200> doc;

    doc["station_id"] = _espID;
    doc["Time"] = _timeClient.getEpochTime();
    
    doc["Temperature"] = _sensorData_st.temperature;
    doc["Humidity"] = _sensorData_st.humidity;
    doc["Pressure"] = _sensorData_st.pressure_u32;
    doc["PM1"]  = _sensorData_st.pm1_u32;
    doc["PM2p5"] = _sensorData_st.pm25_u32;
    doc["PM10"] = _sensorData_st.pm10_u32;
    doc["CO2_PPM"] = _sensorData_st.co_2_u32;

    String output;
    serializeJson(doc, output);
    messageMQTTString = output;
    return ERROR_NONE;
}