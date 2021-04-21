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


int MQTTParseJSON(char* fromDevice)
{
	cJSON* root = NULL;
	cJSON* object = NULL;
	cJSON* item = NULL;
	cJSON* fromDevice_item = NULL;
	
	root = cJSON_Parse(fromDevice);

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
		
		object = cJSON_GetObjectItem(root, "data");
		if ((item = cJSON_GetObjectItem(object, "btnled")) != NULL)
		{
			return 2;
		}
		//memcpy(recv_data.getState, item->valuestring, strlen(item->valuestring));

	}
	return 1;
}


char* MQTTParseJSON_GetDevice(char* fromDevice)
{
	cJSON* root = NULL;
	cJSON* fromDevice_item = NULL;
	root = cJSON_Parse(fromDevice);
	fromDevice_item = cJSON_GetObjectItem(root, "fromDevice");
	
	if (fromDevice == NULL)
	{
		return 0;
	}
	//valuestring是键值对的值，string是键
	return fromDevice_item->valuestring;
}












