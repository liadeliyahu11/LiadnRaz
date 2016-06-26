#pragma once
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include "Room.h"
#include "Game.h"
#include "Helper.h"
using namespace std;

class Room;
class Game;

class User
{
public:
	User(string name,SOCKET sock);
	~User();
	void send(string message);
	void setGame(Game * gm);
	SOCKET getSocket();
	Room * getRoom();
	Game * getGame();
	string getUsername();
	void clearRoom();
	void clearGame();
	bool createRoom(int,string,int,int,int);
	bool joinRoom(Room *);
	void leaveRoom();
	int closeRoom();
	void setRoom(Room * );
	bool leaveGame();
private:
	string _username;
	SOCKET _sock;
	Game * _currGame;
	Room * _currRoom;
};