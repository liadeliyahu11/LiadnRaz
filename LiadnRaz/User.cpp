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
	bool success = true;
	if (_currRoom)
	{
		send("1141");
		success = false;
	}
	else
	{

		Room * rm = new Room(roomId, this, roomName, maxUsers, questionsNo, questionTime);
		_currRoom = rm;
		send("1140");
	}
	return success;
}


bool User::joinRoom(Room * rm)
{
	bool res = true;
	if (_currRoom)
	{
		res = false;
	}
	else
	{
		if (rm->joinRoom(this))
		{
			res = true;
			_currRoom = rm;
		}
	}
	return res;
}


void User::leaveRoom()
{
	if (_currRoom)
	{
		_currRoom->leaveRoom(this);
		_currRoom = nullptr;
	}
}

int User::closeRoom()
{
	int res = -1;
	if (_currRoom)
	{
		res = _currRoom->closeRoom(this);
		delete _currRoom;
		_currRoom = nullptr;
	}
	return res;
}

bool User::leaveGame()
{
	bool f = false;
	if (_currGame)
	{
		f = true;
		_currGame->leaveGame(this);
		_currGame = nullptr;
	}
	return f;
}