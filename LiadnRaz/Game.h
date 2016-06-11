#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "User.h"
#include "DataBase.h"

using namespace std;

class User;
class DataBase;

class Game
{
public:
	Game(const vector<User *>& players,int questionNo,DataBase &db);
	~Game();
	void sendQuestionToAllUsers();
	void handleFinishGame();
	void sendFirstQuestion();
	bool handleNextTurn();
	bool handleAnswerFromUser(User * user,int ansNo,int time);
	bool leaveGame(User * currUser);


private:
	int _id;
	DataBase * _db;
	map<string, int> _results;
	vector<User*> _players;
	vector<Question*> _questions;
	int _currentTurnAnswers;
	int _questionNo;
	int _questionCount;

};