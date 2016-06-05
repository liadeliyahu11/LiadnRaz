#include "User.h"



User::User(string name,SOCKET sock)
{
	_username = name;
	_sock = sock;
	_currGame = nullptr;
	_currRoom = nullptr;
}

User::~User()
{

}

string User::getUsername()
{
	return _username;
}
SOCKET User::getSocket()
{
	return _sock;
}
Room * User::getRoom()
{
	return _currRoom;
}
Game * User::getGame()
{
	return _currGame;
}

void User::send(string msg)
{
	Helper::sendData(_sock, msg);
}

void User::setGame(Game * gm)
{
	_currGame = gm;
	_currRoom = nullptr;
}

void User::clearGame()
{
	_currGame = nullptr;
}


bool User::createRoom(int roomId, string roomName, int maxUsers, int questionsNo, int questionTime)
{
	//create room
	return true;
}




