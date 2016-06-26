#include "TriviaServer.h"
#include <time.h>

void main()
{
	srand(time(NULL));
	try
	{
		TriviaServer ts;
		ts.serv();
	}
	catch (...)
	{

	}

}