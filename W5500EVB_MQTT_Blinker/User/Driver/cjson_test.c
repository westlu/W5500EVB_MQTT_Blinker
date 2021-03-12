#include "include.h"

char* MQTTJSON_Online( char* toDevice)
{
	const char* device = toDevice;
	char * out;
	cJSON* payload = cJSON_CreateObject();
	cJSON* item = cJSON_CreateObject();
	cJSON* next = cJSON_CreateObject();
	cJSON_AddItemToObject(payload, "toDevice", cJSON_CreateString(device));
	cJSON_AddItemToObject(payload, "data", item);
	cJSON_AddItemToObject(item, "state", cJSON_CreateString("online"));
	
	out = cJSON_PrintUnformatted(payload);
	//printf("%s\r\n", out);
	cJSON_Delete(payload);
	return out;
}


int MQTTParseJSON(char *payload_in, blinker_data recv_data)
{
	cJSON* root = NULL;
	cJSON* item = NULL;
	cJSON* fromDevice_item = NULL;
	root = cJSON_Parse(payload_in);

	if (!root)
	{
		printf("error: [%s]\r\n", cJSON_GetErrorPtr());
		return 0;
	}
	else
	{
		//get fromDevice
		if ((fromDevice_item = cJSON_GetObjectItem(root, "fromDevice")) == NULL)
		{
			printf("no fromDevice");
			return 0;
		}
		memcpy(recv_data.fromDevice, fromDevice_item->valuestring, strlen(fromDevice_item->valuestring));


		cJSON* object = cJSON_GetObjectItem(root, "data");
		item = cJSON_GetObjectItem(object, "getState");
		memcpy(recv_data.getState, item->valuestring, strlen(item->valuestring));

	}
	return 1;
}















