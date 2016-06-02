#include "sqlite3.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;


class DataBase
{
	public:

		DataBase() throw(string);
		~DataBase();
		bool isUserExist(string username,char** azCol);
		bool addNewUser(string username, string password, string email);
		bool isUserAndPassMatch(string username, string password);
		//vector<Question*> initQuestions(int questionsNo);
		int insertNewGame();
		bool updateGameStatus(int gameID);
		
	private:
		sqlite3 *db;
		char *zErrMsg = 0;
		
};
/*

bool isUserExist();//if user exist in DB
bool addNewUser(string username,string password,string email);//return success-true else false
bool isUserAndPassMatch(string username,string password);//if exist user with same pass
vector<Question*> initQuestions(int questionsNo);//select number of random questions
int insertNewGame();//return room id
bool updateGameStatus(int gameID);//return success-true else false

*/