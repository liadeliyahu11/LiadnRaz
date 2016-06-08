#pragma once
#include <iostream>
#include <string>
#include "Helper.h"

using namespace std;
class Question
{
public:
	Question(int id,string question , string correctAns, string ans2, string ans3, string ans4);
	~Question();
	string getQuestion();
	string getAns();
	string getAns2();
	string getAns3();
	string getAns4();
	int getCorrect();
	string Question::getMessageToSend();

private:
	string _question;
	int _id;
	int _correctNo;
	string _correctAns;
	string _ans2;
	string _ans3;
	string _ans4;
};
