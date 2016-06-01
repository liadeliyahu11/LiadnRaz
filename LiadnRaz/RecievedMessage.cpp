#include "RecievedMessage.h"



RecievedMessage::RecievedMessage(int typeCode, SOCKET client)
{
	_typeCode = typeCode;
	_client = client;
}


RecievedMessage::RecievedMessage(int typeCode, SOCKET client,vector<string> v)
{
	_data = v;
	_typeCode = typeCode;
	_client = client;
}

