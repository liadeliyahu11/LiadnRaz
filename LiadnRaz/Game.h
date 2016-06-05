#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "User.h"
#include "DataBase.h"

using namespace std;

class Game
{
public:
	Game(const vector<User *>& players,int questionNo,DataBase &db);
	~Game();

private:
	DataBase * _db;
	map<string, int> _results;
	vector<User*> _players;
	vector<Question*> _questions;
	int _currentTurnAnswers;
	int _questionNo;

};