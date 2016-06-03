#include "DataBase.h"
unordered_map<string, vector<string>> results;
pair<string, vector<string>> p;
DataBase::DataBase() throw(string)
{
	int rc;
	rc = sqlite3_open("C:\\Users\\User\\Documents\\GitHub\\LiadnRaz\\LiadnRaz.db", &db);
	if (rc)
	{
		throw("Can't Open database :",sqlite3_errmsg(db));
		sqlite3_close(db);
		system("pause");
	}
}
DataBase::~DataBase()
{
	sqlite3_close(db);
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
bool DataBase::isUserExist(string username,char** azCol)
{
	int rc;
	char *save=new char[99];
	strcpy(save, "select * from accounts where username=");
	strcat(save, username.c_str());
	rc = sqlite3_exec(db,save,callback,0,&zErrMsg);
	if (sizeof(azCol)!= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool DataBase::isUserAndPassMatch(string username, string password, char **azCol)
{
	rc = 0;
	char *sql = helper("select password from accounts where username =", username);
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (*azCol == password.c_str())
	{
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
	rc = 0;
	char *sql = helper(helper(helper(helper("insert into accounts(username,password,email)values(", username), password), email), ")");
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		sqlite3_free(zErrMsg);
		return false;
	}
	else
	{
		return true;
	}
}