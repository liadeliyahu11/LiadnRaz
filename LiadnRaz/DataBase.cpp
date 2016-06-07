#include "DataBase.h"
#include <random>
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
	_dbAddress = "C:\\Users\\User\\Documents\\GitHub\\LiadnRaz\\LiadnRaz.db";
	_rc = sqlite3_open(_dbAddress.c_str(), &_db);
	if (_rc)
	{
		throw("Can't Open database :", sqlite3_errmsg(_db));
		sqlite3_close(_db);
		system("pause");
	}
}

DataBase::DataBase(DataBase &database)
{
	_dbAddress = database._dbAddress;
	_rc = sqlite3_open(_dbAddress.c_str(),&_db);
	if (_rc)
	{
		throw("Can't Open database :", sqlite3_errmsg(_db));
		sqlite3_close(_db);
		system("pause");
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
bool DataBase::isUserExist(string username, char** azCol)
{
	int _rc;
	char *save = new char[99];
	strcpy(save, "select * from t_users where username=");
	strcat(save, username.c_str());
	strcat(save, ";");
	_rc = sqlite3_exec(_db, save, callback, 0, &_zErrMsg);
	if (sizeof(results) != 0)
	{
		clearTable();
		return true;
	}
	else
	{
		return false;
	}
}
bool DataBase::isUserAndPassMatch(string username, string password, char **azCol)
{
	_rc = 0;
	char *sql = helper("select password from t_users where username =", username + ";");
	_rc = sqlite3_exec(_db, sql, callback, 0, &_zErrMsg);
	if (results[password][0] == password)
	{
		clearTable();
		return true;
	}
	else
	{
		return false;
	}

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
	char *sql = helper(helper(helper(helper("insert into t_users(username,password,email)values(", username), password), email), ");");
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
vector<Question*>DataBase::initQuestions(int questionNo)
{
	default_random_engine generator;
	uniform_int_distribution<int> distribution(1,sizeof(results));
	int random;
	vector<Question*> retVec;
	_rc = 0;
	char *sql = "select * from t_questions;";
	_rc = sqlite3_exec(_db, sql, callback, 0, &_zErrMsg);

	for (int i = 0; i < questionNo; i++)
	{
		random = distribution(generator);
		//retVec[i] = new Question();
	}
	return retVec;
}
