#pragma once
#include "Question.h"
#include "sqlite3.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;


class DataBase
{
public:

	DataBase();
	DataBase(DataBase &db);
	~DataBase();
	bool isUserExist(string username, char** azCol);
	bool addNewUser(string username, string password, string email);
	bool isUserAndPassMatch(string username, string password, char **azCol);
	bool addAnswerToPlayer(int gameiId, string username, int questionId, string answer, bool isCorrect, int answerTime);
	vector<Question*> initQuestions(int questionsNo);
	char * helper(char*, string);
	int insertNewGame();
	bool updateGameStatus(int gameID);
	void check();

private:
	string _dbAddress;
	int _rc;
	sqlite3 * _db;
	char * _zErrMsg = 0;
};
