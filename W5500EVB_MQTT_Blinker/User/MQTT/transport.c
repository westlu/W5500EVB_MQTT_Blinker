#include "include.h"


int transport_sendPacketBuffer(int sock, unsigned char* buf, int buflen)
{
	int rc;
	rc = send(sock, buf, buflen);
	return rc;
}


int transport_getdata(unsigned char* buf, int count)
{
	int rc;
	rc = recv(0, buf, count);
	return rc;
}




int transport_open(char* addr, int port)
{
	return 0;
}

int transport_close(int sock)
{
	disconnect(0);
	printf("TCPC socket closed");
	while (getSn_SR(0) != SOCK_CLOSED)
	{

	}
	return 0;
}
