#include "TriviaServer.h"

TriviaServer::TriviaServer()
{
	//run DB c'tor
	int res;
	struct addrinfo hints;
	res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0) {
		printf("WSAStartup failed with error: %d\n", res);
		throw "check socket creation! : first error";
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	res = getaddrinfo(NULL, PORT, &hints, &result);
	if (res != 0) {
		printf("getaddrinfo failed with error: %d\n", res);
		WSACleanup();
		throw "check socket creation! : second error";
	}
	_server = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (_server == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		throw "check socket creation! : third error";
	}

}

TriviaServer::~TriviaServer()
{
	_roomsList.clear();
	_connectedUsers.clear();
	closesocket(_server);
	WSACleanup();

}

void TriviaServer::bindAndListen()
{
	int res = bind(_server, result->ai_addr, (int)result->ai_addrlen);
	if (res == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(_server);
		WSACleanup();
		throw "socket bind error ! : first error";
	}

	freeaddrinfo(result);

	res = listen(_server, SOMAXCONN);
	if (res == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(_server);
		WSACleanup();
		throw "socket listen error ! : first error";
	}
}

void TriviaServer::serv()
{
	bindAndListen();
	while (true)
	{
		tAccept();
	}
}

void TriviaServer::tAccept()
{
	SOCKET client_socket = accept(_server, NULL, NULL);
	if (client_socket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(_server);
		WSACleanup();
		throw "accept error!";
	}
	thread c(&clientHandler);
	cout << "thread created!"<<endl;
}

void TriviaServer::clientHandler(SOCKET client_socket)
{
	bool flag = true;
	while (flag)
	{
		int typeCode = Helper::getMessageTypeCode(client_socket);
		if (typeCode == 0 || typeCode == 299)
		{
			flag = false;
			//create end of connection msg
		}
		else
		{
			try
			{
				buildRecieveMessage(client_socket, typeCode);
			}
			catch (...)
			{
				//create end of connection msg
			}
		}
	}
}