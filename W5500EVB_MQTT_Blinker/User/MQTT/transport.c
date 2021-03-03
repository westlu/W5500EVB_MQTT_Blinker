#include "include.h"

///-------------------------------------------------------------------------------------------------
/// @fn	int transport_sendPacketBuffer(int sock, unsigned char* buf, int buflen)
///
/// @brief	Transport send packet buffer.
///
/// @author	Crawford Cai
/// @date	2021/2/5
///
/// @param 		   	sock  	The sock.
/// @param [in,out]	buf   	If non-null, the buffer.
/// @param 		   	buflen	The buflen.
///
/// @returns	An int.
///-------------------------------------------------------------------------------------------------

int transport_sendPacketBuffer(int sock, unsigned char* buf, int buflen)
{
	int rc;
	rc = send(sock, buf, buflen);
	return rc;
}

///-------------------------------------------------------------------------------------------------
/// @fn	int transport_getdata(unsigned char* buf, int count)
///
/// @brief	Transport getdata.
///
/// @author	Crawford Cai
/// @date	2021/2/5
///
/// @param [in,out]	buf  	If non-null, the buffer.
/// @param 		   	count	Number of.
///
/// @returns	An int.
///-------------------------------------------------------------------------------------------------

int transport_getdata(unsigned char* buf, int count)
{
	int rc;
	rc = recv(0, buf, count);
	return rc;
}

///-------------------------------------------------------------------------------------------------
/// @fn	int transport_open(char* addr, int port)
///
/// @brief	Queries if a given transport open.
///
/// @author	Crawford Cai
/// @date	2021/2/5
///
/// @param [in,out]	addr	If non-null, the address.
/// @param 		   	port	The port.
///
/// @returns	An int.
///-------------------------------------------------------------------------------------------------

int transport_open(char* addr, int port)
{
	return 0;
}

///-------------------------------------------------------------------------------------------------
/// @fn	int transport_close(int sock)
///
/// @brief	Transport close.
///
/// @author	Crawford Cai
/// @date	2021/2/5
///
/// @param 	sock	The sock.
///
/// @returns	An int.
///-------------------------------------------------------------------------------------------------

int transport_close(int sock)
{
	disconnect(0);
	printf("TCPC socket closed");
	while (getSn_SR(0) != SOCK_CLOSED)
	{

	}
	return 0;
}

///-------------------------------------------------------------------------------------------------
/// @fn
/// int MQTTConnectMessage(char* clientID, int keepalive, uint8 cleansession, char* username, char* password, unsigned char*buf,int buflen)
///
/// @brief	Mqtt connect message.
///
/// @author	Crawford Cai
/// @date	2021/2/5
///
/// @param [in,out]	clientID		If non-null, identifier for the client.
/// @param 		   	keepalive   	The keepalive.
/// @param 		   	cleansession	The cleansession.
/// @param [in,out]	username		If non-null, the username.
/// @param [in,out]	password		If non-null, the password.
/// @param [in,out]	buf				If non-null, the buffer.
/// @param 		   	buflen			The buflen.
///
/// @returns	An int.
///-------------------------------------------------------------------------------------------------

int MQTTConnectMessage(char* clientID, int keepalive, uint8 cleansession, char* username, char* password, unsigned char*buf,int buflen)
{
	int rc = -1;
	MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
	connectData.clientID.cstring = clientID;
	connectData.keepAliveInterval = keepalive;
	connectData.cleansession = cleansession;
	connectData.username.cstring = username;
	connectData.password.cstring = password;
	rc = MQTTSerialize_connect(buf, buflen, &connectData);
	return rc;
}


/*订阅*/
void MQTTSubscribeMessage(char* Topic, unsigned char* msgbuf, int buflen)
{
	int msgid = 1;
	int req_qos = 0;
	unsigned char topic[100];
	MQTTString topicString = MQTTString_initializer;
	memcpy(topic, Topic, strlen(Topic));
	topicString.cstring = (char*)topic;
	MQTTSerialize_subscribe(msgbuf, buflen, 0, msgid, 1, &topicString, &req_qos);
	return;
}


/*发布*/
void MQTTPublishMessage(char *Topic, unsigned char *msgbuf, int buflen, char *payload)
{
	unsigned char topic[100];
	int payloadlen = strlen(payload);
	MQTTString topicString = MQTTString_initializer;
	memset(topic, 0, sizeof(topic));
	memcpy(topic, Topic, strlen(Topic));
	topicString.cstring = (char*)topic;
	MQTTSerialize_publish(msgbuf, buflen, 0, 2, 0, 0, topicString, (unsigned char*)payload, payloadlen);
	return;
}




/*解析ACK报文，判断ACK报文类型*/
int MQTTParseHeader(unsigned char *buf)
{
	int rc = -1;
	MQTTHeader header = { 0 };
	header.byte = buf[0];
	rc = header.bits.type;
	return rc;
}






































///-------------------------------------------------------------------------------------------------
// End of F:\github_R\W5500EVB_MQTT_Blinker\W5500EVB_MQTT_Blinker\User\MQTT\transport.c
///-------------------------------------------------------------------------------------------------

