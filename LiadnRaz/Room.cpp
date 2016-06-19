#include "Room.h"


Room::Room(int id,User * admin,string name,int maxUsers,int questionNo,int questionTime)
{
	_id = id;
	_admin = admin;
	_name = name;
	_maxUsers = maxUsers;
	_questionNo = questionNo;
	_questionTime = questionTime;
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

int Room::getId()
{
	return _id;
}

int Room::getTime()
{
	return _questionTime;
}

string Room::getName()
{
	return _name;
}


string Room::getUsersListMessage()
{
	string result="108";
	result += Helper::getPaddedNumber(_users.size(),1);
	for (unsigned int i = 0; i < _users.size();i++)
	{
		string uname = _users[i]->getUsername();
		string len = Helper::getPaddedNumber(uname.size(),2);
		result += len;
		result += uname;
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
		success = true;
	}
	else
	{
		user->send("1101");
	}
	return success;
}

void Room::leaveRoom(User * user)
{
	if (user == _admin)
	{
		for (unsigned int i = 0; i < _users.size(); i++)
		{
			if (_users[i] == user)
			{
				_users.erase(_users.begin() + i);
			}
		}
	}
	else
	{
		for (unsigned int i = 0; i < _users.size(); i++)
		{
			if (_users[i] == user)
			{
				_users[i]->send("1120");
				_users.erase(_users.begin() + i);
			}
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
		int size = _users.size();
		for (unsigned int i = 0; i < size; i++)
		{
			_users[0]->send("116"); 
			_users[0]->leaveRoom();
		}
	}
	return res;
}

vector<User*> Room::getUsers()
{
	return _users;
}

int Room::getquestionNo()
{
	return _questionNo;
}
