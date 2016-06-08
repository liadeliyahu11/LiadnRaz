#include "TriviaServer.h"
#include "DataBase.h"


void main()
{
	try
	{
		//TriviaServer ts;
		//ts.serv();
		DataBase db = DataBase();
		db.check();
	}
	catch (...)
	{

	}

}