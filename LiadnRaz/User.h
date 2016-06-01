#include <iostream>
#include <string>
using namespace std;


class User
{
public:
	User(string name);
	~User();
	string getName();
private:
	string _name;
};