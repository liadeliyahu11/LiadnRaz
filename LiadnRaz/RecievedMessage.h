#include <string>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#pragma comment(lib,"Ws2_32.lib")

using namespace std;

class RecievedMessage
{
public:
	RecievedMessage(int typeCode,SOCKET client);
	RecievedMessage(int typeCode, SOCKET client,vector<string> v);
	~RecievedMessage();

private:
	vector<string> _data;
	int _typeCode;
	SOCKET _client;

};