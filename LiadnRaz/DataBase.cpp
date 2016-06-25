#include "DataBase.h"
#include <random>
#include <ctime>

#define OFFSET (i*6)

unordered_map<string, vector<string>> results;
pair<string, vector<string>> p;

void clearTable()
{
	for (auto it = results.begin(); it != results.end(); ++it)
	{
		it->second.clear();
	}
	results.clear();
}
DataBase::DataBase()
{
	_rc = sqlite3_open("C:\\Users\\User\\Documents\\GitHub\\trivia.db", &_db);
	if (_rc)
	{
		_dbAddress = "C:\\Users\\User\\Documents\\GitHub\\LiadnRaz\\trivia.db";
		_rc = sqlite3_open(_dbAddress.c_str(), &_db);
	}
	if (_rc)
	{
		throw("Can't Open database :", sqlite3_errmsg(_db));
		sqlite3_close(_db);
		system("pause");
	}
}

DataBase::DataBase(DataBase &database)
{
	try
	{
		_dbAddress = database._dbAddress;
		_rc = sqlite3_open(_dbAddress.c_str(), &_db);
		if (_rc)
		{
			throw("Can't Open database :", sqlite3_errmsg(_db));
			sqlite3_close(_db);
			system("pause");
		}
		else
		{
			cout << "sucess open db" << endl;
		}
	}
	catch (...)
	{
		throw "shit";
	}
}

DataBase::~DataBase()
{
	sqlite3_close(_db);
}


bool questionIdNotExist(int id, vector<Question*> vec)
{
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		if (vec[i]->getId() == id)
		{
			return false;
		}
	}
	return true;
}

int callback(void* notUsed, int argc, char** argv, char** azCol)
{
	int i;

	for (i = 0; i < argc; i++)
	{
		auto it = results.find(azCol[i]);
		if (it != results.end())
		{
			it->second.push_back(argv[i]);
		}
		else
		{
			p.first = azCol[i];
			p.second.push_back(argv[i]);
			results.insert(p);
		}
	}

	return 0;
}

bool DataBase::isUserExist(string username)
{
	int _rc;
	char *save = new char[99];
	strcpy(save, "select * from t_users where username='");
	strcat(save, username.c_str());
	strcat(save, "';");
	clearTable();
	_rc = sqlite3_exec(_db, save, callback, 0, &_zErrMsg);
	if (results.size() != 0)
	{
		clearTable();
		return true;
	}
	else
	{
		return false;
	}
}
bool DataBase::isUserAndPassMatch(string username, string password)
{
	clearTable();
	_rc = 0;
	char *sql = helper("select password from t_users where username ='", username+"'");
	_rc = sqlite3_exec(_db, sql, callback, 0, &_zErrMsg);
	if (results.size()>0)
	{
		if ((results.begin()->second.at(0).compare(password)) == 0)
		{
			clearTable();
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;

}
char * DataBase::helper(char* command, string str)
{
	char *save = new char[99];
	strcpy(save, command);
	strcat(save, str.c_str());
	return save;
}
bool DataBase::addNewUser(string username, string password, string email)
{
	_rc = 0;
	char *sql = helper(helper(helper(helper("insert into t_users(username,password,email) values('", username + "','"), password+"','"), email), "');");
	_rc = sqlite3_exec(_db, sql, callback, 0, &_zErrMsg);
	if (_rc != SQLITE_OK)
	{
		sqlite3_free(_zErrMsg);
		return false;
	}
	else
	{
		clearTable();
		return true;
	}
}
char * helpfunc(char * str, int val)
{
	char* saver = new char[99];
	_itoa(val, saver, 10);

	char * result = new char[strlen(saver) + strlen(str)];
	strcpy(result, str);
	strcat(result, saver);
	return result;
}

vector<Question*>DataBase::initQuestions(int questionNo,int usersNo)
{
	int random;
	vector<Question*> retVec;
	_rc = 0;
	int id = 1;
	string question;
	string correctAns;
	string ans2;
	string ans3;
	string ans4;
	_rc = 0;
	for (int i = 0; i < questionNo; i++)
	{
		std::srand(time(0));
		random = rand() % questionNo + 1;
		if (questionIdNotExist(random, retVec))
		{
			string sql = "select * from t_questions where question_id =";
			sql += to_string(random);
			sql += ";";
			clearTable();
			_rc = sqlite3_exec(_db, sql.c_str(), callback, 0, &_zErrMsg);
			id = atoi(results["question_id"][1 + OFFSET + usersNo].c_str());
			question = results["question"][2+ OFFSET + usersNo];
			correctAns = results["correct_ans"][3 + OFFSET + usersNo];
			ans2 = results["ans2"][4 + OFFSET + usersNo];
			ans3 = results["ans3"][5 + OFFSET + usersNo];
			ans4 = results["ans4"][6 + OFFSET + usersNo];
			clearTable();
			retVec.push_back(new Question(id, question, correctAns, ans2, ans3, ans4));
		}
		else
		{
			i--;
		}
		
	}
		
	return retVec;
}

int DataBase::insertNewGame(int usersNo)
{
	int retid = 0;
	try
	{
		time_t result = time(nullptr);
		string time = asctime(localtime(&result));
		_rc = 0;
		string sql = "insert into t_games(status,start_time) values (0,'" + time + "');";
		_rc = sqlite3_exec(_db, sql.c_str(), nullptr, 0, &_zErrMsg);
		if (_rc != SQLITE_OK)
		{
			sqlite3_free(_zErrMsg);
			return false;
		}
		sql = "select last_insert_rowid();";
		_rc = sqlite3_exec(_db, sql.c_str(), callback, 0, &_zErrMsg);
		retid = atoi(results["last_insert_rowid()"][usersNo].c_str());
	}
	catch (...)
	{
		throw "shit";
	}
		retid = atoi(results.begin()->second.at(1).c_str());
		clearTable();
		return retid;
}

void DataBase::check()
{
	
	_rc = 0;
	char *sql = "select question_id from t_questions where question_id = 1;";
	_rc = sqlite3_exec(_db,sql, callback, 0, &_zErrMsg);
	cout << results.begin()->second.at(0).c_str() << endl;
	clearTable();
	system("pause");
}

bool DataBase::updateGameStatus(int gameID)
{
	_rc = 0;
	bool retVal = true;
	time_t result = time(nullptr);
	string time = asctime(localtime(&result));
	_rc = 0;
	string sql = "update t_games set status = 1 where game_id = ";
	sql += to_string(gameID) + ";";
	_rc = sqlite3_exec(_db, sql.c_str(), nullptr, 0, &_zErrMsg);
	if (_rc != SQLITE_OK)
	{
		cout << "SQL error: " << _zErrMsg << endl;
		sqlite3_free(_zErrMsg);
		system("Pause");
		retVal = false;
	}
	sql = "update t_games set end_time ='";
	sql += time;
	sql += "' where game_id =";
	sql += to_string(gameID) + ";";
	_rc = sqlite3_exec(_db, sql.c_str(), nullptr, 0, &_zErrMsg);
	if (_rc != SQLITE_OK)
	{
		cout << "SQL error: " << _zErrMsg << endl;
		sqlite3_free(_zErrMsg);
		system("Pause");
		retVal = false;
	}
	return retVal;
}

bool DataBase::addAnswerToPlayer(int gameId,string username,int questionId,string answer,bool isCorrect,int answerTime)
{
	bool retVal = false;
	try
	{
		_rc = 0;
		string sql = "insert into t_players_answers(game_id,username,question_id,player_answer,is_correct,answer_time) values(";
		sql += to_string(gameId);
		sql += ",'" + username + "'";
		sql += "," + to_string(questionId) + ",";
		sql += "'" + answer + "',";
		sql += to_string(isCorrect) + ",";
		sql += to_string(answerTime);
		sql += ");";
		_rc = sqlite3_exec(_db, sql.c_str(), nullptr, 0, &_zErrMsg);
		if (_rc != SQLITE_OK)
		{
			cout << "SQL error: " << _zErrMsg << endl;
			sqlite3_free(_zErrMsg);
			system("Pause");
			retVal = false;
		}
		else
		{
			retVal = true;
		}
	}
	catch (...)
	{
		throw "another shit";
	}
	return retVal;
}
