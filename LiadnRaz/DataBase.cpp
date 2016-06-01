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