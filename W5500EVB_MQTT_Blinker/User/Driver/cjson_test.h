#ifndef CJSON_TEST_H
#define CJSON_TEST_H

#include "include.h"

struct DATA
{
	char* getState;
};
//
//typedef struct
//{
//	char* fromDevice;
//	char* getState;
//	char* led;
//
//}blinker_data;

 enum jsonMsg
{
	fromDeviceNULL, getState, btnLed
};

char* MQTTJSON_Online(char* toDevice);
int MQTTParseJSON(char* fromDevice);
char* MQTTParseJSON_GetDevice(char* fromDevice);
















#endif
