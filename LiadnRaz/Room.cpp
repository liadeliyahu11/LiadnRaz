#include "Room.h"


Room::Room(int id,User * admin,string name,int maxUsers,int questionNo,int questionTime)
{
	_id = id;
	_admin = admin;
	_name = name;
	_maxUsers = maxUsers;
	_questionNo = questionNo;
	_questionTime = _questionTime;
	_users.push_back(admin);
}


Room::~Room()
{
	_users.clear();
}

string getUsersAsString()
{
	// just for debugging :)
	return "";
}

string Room::getUsersListMessage()
{
	string result="108";
	char numOfUsers = (char)_users.size();
	result += numOfUsers;
	for (unsigned int i = 0; i < _users.size();i++)
	{
		string uname = _users[i]->getUsername();
		string len = to_string(uname.size());
		result += len;
	}
	return result;
}

void Room::sendMessage(User * excludeUser , string msg)
{
	for (unsigned int i = 0; i < _users.size();i++)
	{
		if (_users[i] != excludeUser)
		{
			_users[i]->send(msg);
		}
	}
}

void Room::sendMessage(string msg)
{
	sendMessage(NULL,msg);
}

bool Room::joinRoom(User * user)
{
	bool success = false;
	if (_users.size() < _maxUsers)
	{
		_users.push_back(user);
		sendMessage(getUsersListMessage());
		user->send("1100");
	}
	else
	{
		user->send("1101");
		//1102 in handle
	}
	return success;
}

void Room::leaveRoom(User * user)
{
	for (unsigned int i = 0; i < _users.size();i++)
	{
		if (_users[i] == user)
		{
			_users.erase(_users.begin() + i);
		}
	}
	sendMessage(getUsersListMessage());
}

int Room::closeRoom(User * user)
{
	int res = -1;
	if (user == _admin)
	{
		res = _id;
		for (unsigned int i = 0; i < _users.size(); i++)
		{
			if (_users[i] != _admin)
			{
				_users[i]->clearRoom();
			}
		}
	}
	return res;
}


