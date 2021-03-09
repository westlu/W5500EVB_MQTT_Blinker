#include "include.h"



uint8 PUB_FLAG = 1;

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

uint8 tcpc( uint8 *server_ip)
{
		uint8  CONNECT_FLAG = 1;
		uint8 SUB_FLAG = 1;


		uint16  test_flag = 0;
		uint16	len=0;
		uint8	buffer[1024];												// ����һ��2KB�����飬�������Socket��ͨ������
		int   buflen = sizeof(buffer);
		uint8   datalen = 0;
		uint8 req_qos = 0;
		char *TOPIC = { "/device/41A7E676A8DL98SZNS2AP7AB/r" };
		uint8 recvBuflen = 0;


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
					len = getSn_RX_RSR(TCP_SOCKET);
					if (len == 0)
					{
						len_flag = Sn_RX_RSR_0;
					}

					if (len  > 0)
					{
						len_flag = Sn_RX_RSR_1;
					}

					switch (len_flag)
					{
						case Sn_RX_RSR_0:
							if (CONNECT_FLAG == 1)
							{
								printf("send mqtt_connect\r\n");
								datalen = MQTTConnectMessage("41A7E676A8DL98SZNS2AP7AB", 60, 1,"FWiC2NxKbg9hdyRzPdaHPHUHxb" ,
									"QRqTydZSPvPHhVkhzfTGpyyqPdmBgQcn", buffer, buflen);
								CONNECT_FLAG = 0;
								send(TCP_SOCKET, buffer, datalen);
								//Delay_s(2);
								while ( (len = getSn_RX_RSR(0)) == 0)
								{
								
									Delay_s(2);
									printf("i'm waitting done");
									test_flag = getSn_SR(0);
									printf("len is %d\r\n", len);
									printf("socket statu is: %d\r\n", test_flag);
								}
								
								memset(buffer, 0, buflen);
								printf("recv len is :%d ", len);
								recv(TCP_SOCKET, buffer, len);
								printf("%s\r\n", buffer);
								while (MQTTParseHeader(buffer) != CONNACK)
								{
									printf("waitting for CONNACK!!!");
								}
								


								//sub

							if(SUB_FLAG == 1)
							{


								printf("%s\r\n", buffer);
								memset(buffer, 0, buflen);
								datalen = MQTTSubscribeMessage(TOPIC, buffer, buflen, req_qos);
								send(TCP_SOCKET, buffer, datalen);

								while((len = getSn_RX_RSR(0)) == 0)
								{
									Delay_s(2);
									printf("i'm waitting done - Sub");
								}

								memset(buffer, 0, buflen);
								recv(TCP_SOCKET, buffer, len);
								while (MQTTParseHeader(buffer) != SUBACK)
								{
									printf("waitting for SUBACK!!!");
								}
								while(1)
                {
								   Delay_s(2);
									 printf("subtopic message");
								}


							}

								/**/
							printf("this is the rec buf:%s",buffer);
							Delay_s(5);
							}
							break;

						case Sn_RX_RSR_1:
                 printf("Sn_RX_RSR_1");
							break;

						default:
							break;

					}








					// ���ݻػ����Գ������ݴ���λ������������W5500��W5500���յ����ݺ��ٻظ�������
					//len=getSn_RX_RSR(0);										// len=Socket0���ջ������ѽ��պͱ�������ݴ�С
					//if(len>0)
					//{
					//	recv(0,buffer,len);										// W5500�������Է����������ݣ���ͨ��SPI���͸�MCU
					//	printf("%s\r\n",buffer);							// ���ڴ�ӡ���յ�������
					//	send(0,buffer,len);										// ���յ����ݺ��ٻظ���������������ݻػ�
					//	memset(buffer,0,len);
					//}
					//// W5500�Ӵ��ڷ����ݸ��ͻ��˳����������Իس�����
					//if(USART_RX_STA & 0x8000)								// �жϴ��������Ƿ�������
					//{					   
					//	len=USART_RX_STA & 0x3fff;						// ��ȡ���ڽ��յ����ݵĳ���
					//	send(0,USART_RX_BUF,len);							// W5500��ͻ��˷�������
					//	USART_RX_STA=0;												// ���ڽ���״̬��־λ��0
					//	memset(USART_RX_BUF,0,len+1);					// ���ڽ��ջ�����0
					//}
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

