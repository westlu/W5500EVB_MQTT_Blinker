#ifndef _TCPC_H_
#define _TCPC_H_
#include "include.h"
uint8 tcpc( uint8 *server_ip);
extern unsigned char* fromDevice;
extern uint8 count;



#define TCP_SOCKET 0
//#define DEBUG


//µ˜ ‘”√
#ifdef DEBUG
#define LOG(format,args...)  printf(format,##args)
#else
#define LOG(...)
#endif




#endif
