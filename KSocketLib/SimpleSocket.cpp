#include "SimpleSocket.h"

SimpleSocket::~SimpleSocket()
{
}

ActionResult<SOCKET> SimpleSocket::createSocket(AFFamily family, SocketType sockType, Protocol protocol)
{
	SOCKET sock = INVALID_SOCKET;
	sock = socket((int)family, (int)sockType, (int)protocol);
	bool success = sock != INVALID_SOCKET;
	long code = success ? WSAGetLastError() : 0;
	ActionResult<SOCKET> result(success, code, success ? "" : FormatString("Error at socket(): %ld\n", code));
	return result;
}
