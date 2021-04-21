#include "include.h"




	/*****
		Socket״̬����MCUͨ����Sn_SR(0)��ֵ�����ж�SocketӦ�ô��ں���״̬
		Sn_SR״̬������
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
		uint8	buffer[2048];												// ����һ��2KB�����飬�������Socket��ͨ������
		int     buflen = sizeof(buffer);
		uint8   datalen = 0;
		uint8   req_qos = 0;
		char*   payload;
	    int     payloadlen = strlen(payload);
		char    *TOPIC = { "/device/41A7E676A8DL98SZNS2AP7AB/r" };
		MQTTString topicString = MQTTString_initializer;
	    char    fromDevice[256]; 

		uint16	server_port=1883;								// ����Զ�̷������˿�
		uint16	local_port=6000;									// ��ʼ��һ�����ض˿�
		uint8	len_flag = 0;
		switch(getSn_SR(TCP_SOCKET))														// ��ȡsocket0��״̬
		{
			case SOCK_INIT:															// Socket���ڳ�ʼ�����(��)״̬
					connect(TCP_SOCKET, server_ip,server_port);			// ����Sn_CRΪCONNECT������TCP������������������
			break;
			case SOCK_ESTABLISHED:											// Socket�������ӽ���״̬
					if(getSn_IR(TCP_SOCKET) & Sn_IR_CON)
					{
						setSn_IR(TCP_SOCKET, Sn_IR_CON);								// Sn_IR��CONλ��1��֪ͨW5500�����ѽ���
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
							//����subscribe�������ܶ��ĵ���Ϣ
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
			case SOCK_CLOSE_WAIT:												// Socket���ڵȴ��ر�״̬
					close(TCP_SOCKET);																// �ر�Socket0
			break;
			case SOCK_CLOSED:														// Socket���ڹر�״̬
					socket(TCP_SOCKET,Sn_MR_TCP,local_port,Sn_MR_ND);		// ��Socket0��������ΪTCP����ʱģʽ����һ�����ض˿�
			break;
		
		}
		return 0;
}

