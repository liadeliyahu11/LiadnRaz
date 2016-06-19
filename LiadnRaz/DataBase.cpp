#include "DataBase.h"
#include <random>
#include <ctime>

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
vector<Question*>DataBase::initQuestions(int questionNo)
{
	default_random_engine generator;
	uniform_int_distribution<int> distribution(1,questionNo);
	int random;
	vector<Question*> retVec;
	_rc = 0;
	int id = 0;
	string question;
	string correctAns;
	string ans2;
	string ans3;
	string ans4;
	_rc = 0;
	for (int i = 0; i < questionNo; i++)
	{
		random = distribution(generator);
		string sql = "select * from t_questions where question_id =";
		sql += to_string(random);
		sql += ";";
		clearTable();
		_rc = sqlite3_exec(_db,sql.c_str() , callback, 0, &_zErrMsg);
		id = atoi(results["ans4"][1].c_str());
		question = results["ans4"][2];
		correctAns = results["ans4"][3];
		ans2 = results["ans4"][4];
		ans3 = results["ans4"][5];
		ans4 = results["ans4"][6];
		clearTable();
		retVec.push_back(new Question(id,question,correctAns,ans2,ans3,ans4));
	}
	return retVec;
}
int DataBase::insertNewGame()
{
	int retid = 0;
	try
	{
		time_t result = time(nullptr);
		string time = asctime(localtime(&result));
		_rc = 0;
		string sql = "insert into t_games(status,start_time) values (0," + time + ");";
		_rc = sqlite3_exec(_db, sql.c_str(), nullptr, 0, &_zErrMsg);
		if (_rc != SQLITE_OK)
		{
			sqlite3_free(_zErrMsg);
			return false;
		}
		sql = "select game_id from t_games where game_id = last_insert_rowid();";
		_rc = sqlite3_exec(_db, sql.c_str(), callback, 0, &_zErrMsg);
	}
	catch (...)
	{
		throw "shit";
	}
		retid = stoi(results.begin()->second.at(0),nullptr,10);
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
	char * sql = "update t_games set status = 1 where game_id = ";
	helpfunc(sql, gameID);
	strcat(sql, ");");
	_rc = sqlite3_exec(_db, sql, nullptr, 0, &_zErrMsg);
	if (_rc != SQLITE_OK)
	{
		cout << "SQL error: " << _zErrMsg << endl;
		sqlite3_free(_zErrMsg);
		system("Pause");
		retVal = false;
	}
	sql = "update t_games set end_time =";
	helper(sql, time);
	helper(sql, "where game_id =");
	helpfunc(sql, gameID);
	strcat(sql, ";");
	_rc = sqlite3_exec(_db, sql, nullptr, 0, &_zErrMsg);
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
	_rc = 0;
	bool retVal = true;
	string sql = "insert into t_players_answers(game_id,username,question_id,player_answer,is_corret,answer_time) values(";
	sql += to_string(gameId);
	sql += username;
	sql += to_string(questionId);
	sql += answer;
	sql += to_string(isCorrect);
	sql += to_string(answerTime);
	sql += ";";
	_rc = sqlite3_exec(_db,sql.c_str(),nullptr,0,&_zErrMsg);
	if (_rc != SQLITE_OK)
	{
		cout << "SQL error: " << _zErrMsg << endl;
		sqlite3_free(_zErrMsg);
		system("Pause");
		retVal = false;
	}
	return retVal;
}