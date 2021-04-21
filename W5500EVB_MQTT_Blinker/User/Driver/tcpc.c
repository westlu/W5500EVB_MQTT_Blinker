#include "include.h"




	/*****
		Socket状态机，MCU通过读Sn_SR(0)的值进行判断Socket应该处于何种状态
		Sn_SR状态描述：
		0x00		SOCK_CLOSED
		0x13		SOCK_INIT
		0x14		SOCK_LISTEN
		0x17		SOCK_ESTABLISHED
		0x1C		SOCK_CLOSE_WAIT
		0x22		SOCK_UDP
	*****/

uint8  CONNECT_FLAG = 1;
uint8 SUB_FLAG = 1;
uint8 count = 0;

uint8 tcpc( uint8 *server_ip)
{



		uint16  test_flag = 0;
		uint16	len=0;
		uint8	buffer[2048];												// 定义一个2KB的数组，用来存放Socket的通信数据
		int     buflen = sizeof(buffer);
		uint8   datalen = 0;
		uint8   req_qos = 0;
		char*   payload;
	    int     payloadlen = strlen(payload);
		char    *TOPIC = { "/device/41A7E676A8DL98SZNS2AP7AB/r" };
		MQTTString topicString = MQTTString_initializer;
	    char    fromDevice[256]; 

		uint16	server_port=1883;								// 配置远程服务器端口
		uint16	local_port=6000;									// 初始化一个本地端口
		uint8	len_flag = 0;
		switch(getSn_SR(TCP_SOCKET))														// 获取socket0的状态
		{
			case SOCK_INIT:															// Socket处于初始化完成(打开)状态
					connect(TCP_SOCKET, server_ip,server_port);			// 配置Sn_CR为CONNECT，并向TCP服务器发出连接请求
			break;
			case SOCK_ESTABLISHED:											// Socket处于连接建立状态
					if(getSn_IR(TCP_SOCKET) & Sn_IR_CON)
					{
						setSn_IR(TCP_SOCKET, Sn_IR_CON);								// Sn_IR的CON位置1，通知W5500连接已建立
					}
					memset(buffer,0,sizeof(buffer));

					
					
					if ((len = getSn_RX_RSR(TCP_SOCKET)) == 0)
					{
						if (CONNECT_FLAG == 1)
						{
							LOG("send mqtt_connect\r\n");
							
							datalen = MQTTConnectMessage("41A7E676A8DL98SZNS2AP7AB", 60, 1, "BnDyekdeMfhXpwBUZS7CXUk2Vr",
							"Mk4uKHLrkLxdtQYQCAxBmnGnzuJHPdZY", buffer, buflen);
							CONNECT_FLAG = 0;
							send(TCP_SOCKET, buffer, datalen);
							while ((len = getSn_RX_RSR(0)) == 0)
							{
								LOG("i'm waitting done");
							}

							memset(buffer, 0, buflen);
							LOG("recv len is :%d ", len);
							recv(TCP_SOCKET, buffer, len);
							while (MQTTParseHeader(buffer) != CONNACK)
							{
								LOG("waitting for CONNACK!!!");
							}
						}
						//sub

						if (SUB_FLAG == 1)
						{
							//发送subscribe包，接受订阅的消息
							memset(buffer, 0, buflen);
							datalen = MQTTSubscribeMessage(TOPIC, buffer, buflen, req_qos);
							send(TCP_SOCKET, buffer, datalen);
							SUB_FLAG = 0;
							while ((len = getSn_RX_RSR(0)) == 0)
							{
								LOG("i'm waitting done - Sub");
							}

							memset(buffer, 0, buflen);
							recv(TCP_SOCKET, buffer, len);
							while (MQTTParseHeader(buffer) != SUBACK)
							{
								LOG("waitting for SUBACK!!!");
							}


							topicString.cstring = "/device/41A7E676A8DL98SZNS2AP7AB/s";

							payload = MQTTJSON_Online("2146c54356de11eb81bb5254");
							payloadlen = strlen(payload);
							TIM_Cmd(TIM3, ENABLE);
							while(1)
							{
								
								if(getSn_RX_RSR(TCP_SOCKET) > 0)
								{
									if (MQTTPacket_read(buffer, buflen, transport_getdata) == PUBLISH)
									{
										unsigned char dup;
										int qos;
										unsigned char retained;
										unsigned short msgid;
										int payloadlen_in;
										unsigned char* payload_in;
										int rc;
										MQTTString receivedTopic;
										rc = MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic,
										&payload_in, &payloadlen_in, buffer, buflen);
										printf("message arrived %.*s\r\n", payloadlen_in, payload_in);
										//									memcpy(fromDevice,payload_in,payloadlen_in);
										//									printf("recieve copy: %s\r\n  ",fromDevice );
										//									printf("Device:%s\r\n", MQTTParseJSON_GetDevice(fromDevice));
										
									}
								}


								if(count > 4)
								{
									LOG("payload be sent : %s\r\n", payload);
									len = MQTTSerialize_publish(buffer, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);
									send(TCP_SOCKET, buffer, len);
									LOG("have been sent");
									count = 0;
								}

							}
								


						}



					}


			break;
			case SOCK_CLOSE_WAIT:												// Socket处于等待关闭状态
					close(TCP_SOCKET);																// 关闭Socket0
			break;
			case SOCK_CLOSED:														// Socket处于关闭状态
					socket(TCP_SOCKET,Sn_MR_TCP,local_port,Sn_MR_ND);		// 打开Socket0，并配置为TCP无延时模式，打开一个本地端口
			break;
		
		}
		return 0;
}

