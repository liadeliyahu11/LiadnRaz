#pragma once
#include "Question.h"
#include "sqlite3.h"
#include <iostream>
#include <string>
#include <vector>
#include "User.h"
#include <unordered_map>
using namespace std;

class User;


class DataBase
{
public:

	DataBase();
	DataBase(DataBase &db);
	~DataBase();
	bool isUserExist(string username);
	bool addNewUser(string username, string password, string email);
	bool isUserAndPassMatch(string username, string password);
	bool addAnswerToPlayer(int gameiId, string username, int questionId, string answer, bool isCorrect, int answerTime);
	vector<Question*> initQuestions(int questionsNo);
	char * helper(char*, string);
	int insertNewGame(int usersNo);
	bool updateGameStatus(int gameID);
	void check();
	int userCorrectAnswers(int gameid, string username, int qNo);
	vector<string> getPersonalStatus(string username);
	vector<string> getBestScores();
private:
	string _dbAddress;
	int _rc;
	sqlite3 * _db;
	char * _zErrMsg = 0;
};
