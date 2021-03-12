#ifndef CJSON_TEST_H
#define CJSON_TEST_H

#include "include.h"

struct DATA
{
	char* getState;
};

typedef struct
{
	char* fromDevice;
	char* getState;
	struct DATA data;
}blinker_data;




char* MQTTJSON_Online(char* toDevice);
int MQTTParseJSON(char *payload, blinker_data recv_data);

















#endif
