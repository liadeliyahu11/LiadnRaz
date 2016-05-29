#include <iostream>
#include <string>
#include <map>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <queue>
#include <vector>
#pragma comment(lib,"Ws2_32.lib")
using namespace std;


class TriviaServer
{
private:
	map<SOCKET, string> _connectedUsers;//string has to change to "User*"
	map<int, string> _roomsList;//string has to change to "Room*"
	queue<string> _queRcvMessages;//string has to change to "RecievedMessage*"
	static int _roomIdSequence;

public:
	TriviaServer();//run DB constructor
	~TriviaServer();//delete rooms,users,sockets
	void serv();
	void bindAndListen();
	void accept();
	void clientHandler(SOCKET client_socket);
	Room * getRoomById(int id);
	User * getUserByName(string username);
	User * getUserBySocket(SOCKET client_socket);
	void handleRecievedMessages();
	void safeDeleteUesr(RecievedMessage* msg);
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


};