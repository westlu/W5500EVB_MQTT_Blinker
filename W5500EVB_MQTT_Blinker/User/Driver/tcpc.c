#include "include.h"



uint8 PUB_FLAG = 1;

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

uint8 tcpc( uint8 *server_ip)
{
		uint8  CONNECT_FLAG = 1;
		uint8 SUB_FLAG = 1;


		uint16  test_flag = 0;
		uint16	len=0;
		uint8	buffer[1024];												// 定义一个2KB的数组，用来存放Socket的通信数据
		int   buflen = sizeof(buffer);
		uint8   datalen = 0;
		uint8 req_qos = 0;
		char *TOPIC = { "/device/41A7E676A8DL98SZNS2AP7AB/r" };
		uint8 recvBuflen = 0;


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








					// 数据回环测试程序：数据从上位机服务器发给W5500，W5500接收到数据后再回给服务器
					//len=getSn_RX_RSR(0);										// len=Socket0接收缓存中已接收和保存的数据大小
					//if(len>0)
					//{
					//	recv(0,buffer,len);										// W5500接收来自服务器的数据，并通过SPI发送给MCU
					//	printf("%s\r\n",buffer);							// 串口打印接收到的数据
					//	send(0,buffer,len);										// 接收到数据后再回给服务器，完成数据回环
					//	memset(buffer,0,len);
					//}
					//// W5500从串口发数据给客户端程序，数据需以回车结束
					//if(USART_RX_STA & 0x8000)								// 判断串口数据是否接收完成
					//{					   
					//	len=USART_RX_STA & 0x3fff;						// 获取串口接收到数据的长度
					//	send(0,USART_RX_BUF,len);							// W5500向客户端发送数据
					//	USART_RX_STA=0;												// 串口接收状态标志位清0
					//	memset(USART_RX_BUF,0,len+1);					// 串口接收缓存清0
					//}
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

