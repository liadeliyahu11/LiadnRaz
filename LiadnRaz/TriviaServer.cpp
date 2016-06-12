#include "TriviaServer.h"


int TriviaServer::_roomIdSequence = 0;
int TriviaServer::nextId = 1;
bool TriviaServer::ready = false;

TriviaServer::TriviaServer()
{
	active = true;
	thread t(&TriviaServer::handleRecievedMessages,this);
	t.detach();
	_db = new DataBase();
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
	delete _db;
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
	while (active)
	{
		tAccept();
	}
}

void TriviaServer::clientHandler(SOCKET client_socket)
{
	bool flag = true;
	while (flag)
	{
		try
		{
			int typeCode = Helper::getMessageTypeCode(client_socket);
			if (typeCode != 0 && typeCode != 299)
			{
				buildRecieveMessage(client_socket, typeCode);
			}
			else
			{
				flag = false;
				cout << typeCode << endl;
				addEndConnection(client_socket);
			}
		}
		catch (...)
		{
			flag = false;
			addEndConnection(client_socket);
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
	ready = true;
	_cv.notify_one();
}

void TriviaServer::buildRecieveMessage(SOCKET client_socket, int msgCode)
{
	vector<string> data;
	if (msgCode == 200 || msgCode == 203 || msgCode == 207 ||
		msgCode == 209 || msgCode == 213 || msgCode == 219)
	{
		if (msgCode == 200)
		{
			int usernameLength = atoi(Helper::getPartFromSocket(client_socket, 2, 0));
			data.push_back(Helper::getStringPartFromSocket(client_socket, usernameLength));
			int passLength = atoi(Helper::getPartFromSocket(client_socket, 2, 0));
			data.push_back(Helper::getStringPartFromSocket(client_socket, passLength));
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
	}
	RecievedMessage * rm = new RecievedMessage(msgCode, client_socket, data);
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
	for (std::map<SOCKET, User*>::iterator it = _connectedUsers.begin(); it != _connectedUsers.end() && !found; ++it)
	{
		found = true;
		if (it->second->getUsername().compare(name) == 0)
		{
			res = it->second;
		}
	}
	return res;
}


User * TriviaServer::getUserBySocket(SOCKET cs)
{
	User * res = nullptr;
	if (_connectedUsers.find(cs) != _connectedUsers.end())
	{
		res = _connectedUsers.at(cs);
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
	while (active)
	{
		User * user;
		unique_lock<mutex> lk(que_mutex);
		_cv.wait(lk, [&]{return ready; });
		RecievedMessage * rm = _queRcvMessages.front();
		_queRcvMessages.pop();
		rm->setUser(getUserBySocket(rm->getSocket()));
		try
		{
			int code = rm->getCode();
			switch (code)
			{
			case 200:
				user = handleSignin(rm);
				if (user)
				{
					_connectedUsers[rm->getSocket()] = user;
				}
				else
				{
					delete user;
				}
				break;
			case 201:
				handleSignout(rm);
				break;
			case 203:
				handleSignup(rm);
				break;
			case 205:
				//handleGetRooms(rm);
				break;
			case 207:
				//handleGetUsersInRoom(rm);
				break;
			case 209:
				//handleJoinRoom(rm);
				break;
			case 211:
				handleLeaveRoom(rm);
				break;
			case 213:
				//handleCreateRoom(rm);
				break;
			case 215:
				handleCloseRoom(rm);
				break;
			case 217:
				//handleStartGame(rm);
				break;
			case 219:
				//handlePlayerAnswer(rm);
				break;
			case 222:
				//handleLeaveGame(rm);
				break;
			case 223:
				//handleGetBestScores(rm);
				break;
			case 225:
				//handleGetPersonalStatus(rm);
				break;
			default:
				safeDeleteUser(rm);
				break;
			}
		}
		catch (...)
		{
			safeDeleteUser(rm);
		}
		ready = false;
		lk.unlock();
	}
}


bool TriviaServer::deleteFromUsers(SOCKET s)
{
	bool f = false;
	map<SOCKET,User*>::iterator it;
	it = _connectedUsers.find(s);
	if (it != _connectedUsers.end())
	{
		_connectedUsers.erase(it);
		f = true;
	}
	return f;
}

void TriviaServer::handleSignout(RecievedMessage * rm)
{
	deleteFromUsers(rm->getSocket());
	handleCloseRoom(rm);
	handleLeaveRoom(rm);
	handleLeaveGame(rm);

}

User * TriviaServer::handleSignin(RecievedMessage * rm)
{
	if (_db->isUserAndPassMatch((rm->getData()[0]), (rm->getData()[1])))
	{
		if (getUserByName(rm->getData()[0]))
		{
			cout << "this user already connected" << endl;
			Helper::sendData(rm->getSocket(), "1022");
		}
		else
		{
			User * userN = new User(rm->getData()[0],rm->getSocket());
			_connectedUsers[rm->getSocket()] = userN;
			cout << "sucess to connect :)" << endl;
			Helper::sendData(rm->getSocket(), "1020");
			return userN;
		}
	}
	Helper::sendData(rm->getSocket(), "1021");
	return nullptr;
}

bool TriviaServer::handleSignup(RecievedMessage * msg)
{
	bool retVal = true;
	if (Valid::isPasswordValid(msg->getData()[1]))
	{
		if (Valid::isUsernameValid(msg->getData()[0]))
		{
			if (_db->isUserExist(msg->getData()[0]))
			{
				cout << "the user is alredy exists" << endl;
			}
			else
			{
				if (_db->addNewUser(msg->getData()[0], msg->getData()[1], msg->getData()[2]))
				{
					cout << "sucess to create the user" << endl;
				}
				else
				{
					cout << "fail to create the user" << endl;
					retVal = false;
				}
			}
		}
		else
		{
			cout << "the username isn't vaild" << endl;
			retVal = false;
		}
	}
	else
	{
		cout << "the password isn't vaild" << endl;
		retVal = false;
	}
	return retVal;
}
bool TriviaServer::handleCloseRoom(RecievedMessage * rm)
{
	map<int, Room*>::iterator it;
	bool ret = false;
	User * user = getUserBySocket(rm->getSocket());
	if (user != nullptr)
	{
		Room * room = user->getRoom();
		if (room != nullptr)
		{
			ret = true;
			int id = user->closeRoom();
			if (id != -1)
			{
				it = _roomsList.find(id);
				if (it != _roomsList.end())
				{
					_roomsList.erase(it);
				}
			}
		}
	}
	return ret;
}

bool TriviaServer::handleLeaveRoom(RecievedMessage * rm)
{
	bool ret = false;
	User * user = getUserBySocket(rm->getSocket());
	if (user != nullptr)
	{
		Room * room = user->getRoom();
		if (room != nullptr)
		{
			ret = true;
			user->leaveRoom();
		}
	}
	return ret;
}

void TriviaServer::handleLeaveGame(RecievedMessage * rm)
{
	User * user = getUserBySocket(rm->getSocket());
	if (user != nullptr)
	{
		if (user->leaveGame())
		{
			user->clearGame();
		}
	}
}
void TriviaServer::handleStartGame(RecievedMessage* rm)
{
	try
	{
		User * user = getUserBySocket(rm->getSocket());
		vector<User*> a = user->getRoom()->getUsers();
		Game * nGame = new Game(a,user->getRoom()->getquestionNo(), _db);
		handleCloseRoom(rm);
		nGame->sendFirstQuestion();
	}
	catch (...)
	{
		cout << "failed to create new game" << endl;
	}
}
void TriviaServer::handlePlayerAnswer(RecievedMessage * rm)
{
	User * user = getUserBySocket(rm->getSocket());
	if (user->getGame() != nullptr)
	{
		if (user->getGame()->handleAnswerFromUser(user, stoi(rm->getData()[0]), stoi(rm->getData()[1])) == false)
		{
			getUserBySocket(rm->getSocket())->getGame()->~Game();
		}
	}
}
bool TriviaServer::handleCreateRoom(RecievedMessage * rm)
{
	bool retVal = false;
	User * nUser = getUserBySocket(rm->getSocket());
	if (nUser != nullptr)
	{
		if (nUser->createRoom(TriviaServer::nextId, rm->getData()[0], stoi(rm->getData()[1]), stoi(rm->getData()[2]), stoi(rm->getData()[3])))
		{
			TriviaServer::nextId++;
			TriviaServer::_roomIdSequence++;
			_roomsList[TriviaServer::nextId] = nUser->getRoom();
			retVal = true;
		}
	}
	return retVal;
}