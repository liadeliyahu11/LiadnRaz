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
	int res = ::bind(_server, result->ai_addr, (int)result->ai_addrlen);
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


void TriviaServer::tAccept()
{
	SOCKET client_socket = accept(_server, NULL, NULL);
	if (client_socket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(_server);
		WSACleanup();
		throw "accept error!";
	}
	thread c(&TriviaServer::clientHandler,this ,client_socket);
	c.detach();
	cout << "thread created!" << endl;
}

void TriviaServer::serv()
{
	bindAndListen();
	while (true)
	{
		tAccept();
	}
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
			cout << typeCode << endl;
			addEndConnection(client_socket);
		}
		else
		{
			try
			{
				buildRecieveMessage(client_socket, typeCode);
				
			}
			catch (...)
			{
				addEndConnection(client_socket);
			}
		}
	}
}

void TriviaServer::addEndConnection(SOCKET cs)
{
	RecievedMessage * rm = new RecievedMessage(299,cs);
	addRecievedMessage(rm);
}

void TriviaServer::addRecievedMessage(RecievedMessage * rm)
{
	_queRcvMessages.push(rm);
	//cv Wake Up
}

void TriviaServer::buildRecieveMessage(SOCKET client_socket, int msgCode)
{
	vector<string> data;
	RecievedMessage * rm = new RecievedMessage(msgCode, client_socket);
	if (msgCode == 200 || msgCode == 203 || msgCode == 207 ||
		msgCode == 209 || msgCode == 213 || msgCode == 219)
	{
		if (msgCode == 200)
		{
			int usernameLength = atoi(Helper::getPartFromSocket(client_socket, 2, 0));
			data[0] = Helper::getStringPartFromSocket(client_socket, usernameLength);
			int passLength = atoi(Helper::getPartFromSocket(client_socket, 2, 0));
			data[1] = Helper::getStringPartFromSocket(client_socket, passLength);
		}
		else if (msgCode == 203)
		{
			int usernameLength = atoi(Helper::getPartFromSocket(client_socket, 2, 0));
			data.push_back( Helper::getStringPartFromSocket(client_socket, usernameLength));
			int passLength = atoi(Helper::getPartFromSocket(client_socket, 2, 0));
			data.push_back(Helper::getStringPartFromSocket(client_socket, passLength));
			int emailLength = atoi(Helper::getPartFromSocket(client_socket, 2, 0));
			data.push_back(Helper::getStringPartFromSocket(client_socket, emailLength));
		}
		else if (msgCode == 209 || msgCode == 207)
		{
			data.push_back(Helper::getStringPartFromSocket(client_socket, 4));
		}
		else if (msgCode == 213)
		{
			int roomNameLength = atoi(Helper::getPartFromSocket(client_socket, 2, 0));
			data.push_back(Helper::getStringPartFromSocket(client_socket, roomNameLength));
			data.push_back(Helper::getStringPartFromSocket(client_socket, 1));
			data.push_back(Helper::getStringPartFromSocket(client_socket, 2));
			data.push_back(Helper::getStringPartFromSocket(client_socket, 2));
		}
		else
		{
			data.push_back(Helper::getStringPartFromSocket(client_socket, 1));
			data.push_back(Helper::getStringPartFromSocket(client_socket, 2));
		}
		delete rm;
		RecievedMessage * rm = new RecievedMessage(msgCode,client_socket,data);
	}
	addRecievedMessage(rm);
}

Room * TriviaServer::getRoomById(int id)
{
	Room * res = nullptr;
	if (_roomsList.find(id) != _roomsList.end())
	{
		res = _roomsList.at(id);
	}
	return res;
}

User * TriviaServer::getUserByName(string name)
{
	bool found = false;
	User * res = nullptr;
	for (std::map<SOCKET, User>::iterator it = _connectedUsers.begin(); it != _connectedUsers.end() && !found; ++it)
	{
		found = true;
		if (it->second.getName().compare(name) == 0)
		{
			res = &it->second;
		}
	}
	return res;
}


User * TriviaServer::getUserBySocket(SOCKET cs)
{
	User * res = nullptr;
	if (_connectedUsers.find(cs) != _connectedUsers.end())
	{
		res = &_connectedUsers.at(cs);
	}
	return res;
}

void TriviaServer::safeDeleteUser(RecievedMessage * msg)
{
	try
	{

		handleSignout(new RecievedMessage(209, msg->getSocket()));//signout
		closesocket(msg->getSocket());
	}
	catch (...)
	{
		cout << "safeDeleteUser error!" << endl;
	}
}

void TriviaServer::handleRecievedMessages()
{
	
}


bool deleteFromUsers(map<SOCKET,User>  myMap,SOCKET s)
{
	bool f = false;
	std::map<SOCKET, User>::iterator itr = myMap.begin();
	while (itr != myMap.end() && !f) {
		if (itr->first == s) {
			std::map<SOCKET, User> ::iterator toErase = itr;
			++itr;
			myMap.erase(toErase);
			f = true;
		}
		else {
			++itr;
		}
	}
	return f;
}


void TriviaServer::handleSignout(RecievedMessage * rm)
{
	deleteFromUsers(_connectedUsers, rm->getSocket());
	handleCloseRoom(rm);
	handleLeaveRoom(rm);
	handleLeaveGame(rm);

}

bool TriviaServer::handleCloseRoom(RecievedMessage * rm)
{
	// i need to create user class 
}


