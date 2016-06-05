#pragma once
#include <iostream>
#include <string>
using namespace std;
class Question
{
public:
	Question(int id, string correctAns, string ans2, string ans3, string ans4);
	~Question();

private:
	string _id;
	string _correctAns;
	string _ans2;
	string _ans3;
	string _ans4;
};
