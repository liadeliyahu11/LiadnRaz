#include <string>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include "User.h"
#pragma comment(lib,"Ws2_32.lib")

using namespace std;

class User;

class RecievedMessage
{
public:
	RecievedMessage(int typeCode,SOCKET client);
	RecievedMessage(int typeCode, SOCKET client,vector<string> v);
	~RecievedMessage();
	vector<string> getData();
	void setUser(User *);
	User * getUser();
	int getCode();
	SOCKET getSocket();

private:
	vector<string> _data;
	int _typeCode;
	SOCKET _client;
	User * _user;
};