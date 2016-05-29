#include<iostream>
#include<string>
#include "valid.h"
using namespace std;
void main()
{
	Valid check;
	string username = "Admin";
	string password = "Aa231";
	cout << "username is correct ?" << check.isUsernameValid(username) << endl;
	cout << "password is correct ?" << check.isPasswordValid(password) << endl;
	system("pause");
}