#pragma once
#include <iostream>
#include <string>
#include <map>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "RecievedMessage.h"
#include "Helper.h"
#include "valid.h"
#include "Room.h"
#include "User.h"
#include "DataBase.h"

#pragma comment(lib,"Ws2_32.lib")
using namespace std;


#define PORT "8820"

class TriviaServer
{
private:
	DataBase * _db;
	SOCKET _server;
	WSADATA wsaData;
	struct addrinfo *result = NULL;
	map<SOCKET, User*> _connectedUsers;
	map<int, Room*> _roomsList;
	queue<RecievedMessage *> _queRcvMessages;
	mutex que_mutex;
	condition_variable _cv;
	bool active;

public:
	static int nextId;
	static int _roomIdSequence;
	static bool ready;
	TriviaServer();//run DB constructor
	~TriviaServer();//delete rooms,users,sockets
	void serv();
	void bindAndListen();
	void tAccept();
	void clientHandler(SOCKET client_socket);
	Room * getRoomById(int id);
	User * getUserByName(string username);
	User * getUserBySocket(SOCKET client_socket);
	void handleRecievedMessages();
	void safeDeleteUser(RecievedMessage* msg);
	User * handleSignin(RecievedMessage* msg);
	void handleSignout(RecievedMessage* msg);
	bool handleSignup(RecievedMessage* msg);
	void handleLeaveGame(RecievedMessage* msg);
	void handleStartGame(RecievedMessage* msg);
	void handlePlayerAnswer(RecievedMessage* msg);
	bool handleCreateRoom(RecievedMessage* msg);
	bool handleCloseRoom(RecievedMessage* msg);
	bool handleJoinRoom(RecievedMessage* msg);
	bool handleLeaveRoom(RecievedMessage* msg);
	void handleGetUsersInRoom(RecievedMessage* msg);
	void handleGetRooms(RecievedMessage* msg);
	void handleGetBestScores(RecievedMessage* msg);
	void handleGetPersonalStatus(RecievedMessage* msg);
	void addRecievedMessage(RecievedMessage* msg);
	void buildRecieveMessage(SOCKET client_socket, int msgCode);
	// aditional:
	void addEndConnection(SOCKET cs);
	bool deleteFromUsers(SOCKET s);
	bool deleteFromRooms(Room *);

};