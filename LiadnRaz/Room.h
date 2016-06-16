#pragma once
#include <iostream>
#include "User.h"
#include <vector>
#include <string>

using namespace std;
class User;

class Room
{
public:
	Room(int id, User * admin, string name, int maxUsers, int questionNo, int questionTime);
	~Room();
	string getUsersListMessage();
	void sendMessage(User * excludeUser, string msg);
	void sendMessage(string msg);
	bool joinRoom(User * user);
	void leaveRoom(User * user);
	int closeRoom(User * );
	vector<User*> getUsers();
	string getName();
	int getId();
	int getquestionNo();
	int getTime();

	

private:
	vector<User*> _users;
	User * _admin;
	int _maxUsers;
	int _questionTime;
	int _questionNo;
	string _name;
	int _id;
};