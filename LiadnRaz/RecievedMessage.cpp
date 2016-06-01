#include "RecievedMessage.h"



RecievedMessage::RecievedMessage(int typeCode, SOCKET client)
{
	_typeCode = typeCode;
	_client = client;
}


RecievedMessage::~RecievedMessage()
{
	_data.clear();
}


RecievedMessage::RecievedMessage(int typeCode, SOCKET client,vector<string> v)
{
	_data = v;
	_typeCode = typeCode;
	_client = client;
}

int RecievedMessage::getCode()
{
	return _typeCode;
}

SOCKET RecievedMessage::getSocket()
{
	return _client;
}

vector<string> RecievedMessage::getData()
{
	return _data;
}