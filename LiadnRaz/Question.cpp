#include "Question.h"




Question::Question(int id,string question , string correctAns,string ans2,string ans3,string ans4)
{
	_id = id;
	_correctNo = 0;
	_question = question;
	_correctAns = correctAns;
	_ans2 = ans2;
	_ans3 = ans3;
	_ans4 = ans4;
}

Question::~Question()
{
	_correctAns = nullptr;
	_ans2 = nullptr;
	_ans3 = nullptr;
	_ans4 = nullptr;
}

string Question::getQuestion()
{
	return _question;
}

int Question::getCorrect()
{
	return _correctNo;
}

string Question::getMessageToSend()
{
	_correctNo = 1;
	string toSend = "118";
	toSend += Helper::getPaddedNumber(_question.size(), 3);
	toSend += _question;

	toSend += Helper::getPaddedNumber(_correctAns.size(), 3);
	toSend += _correctAns;

	toSend += Helper::getPaddedNumber(_ans2.size(), 3);
	toSend += _ans2;

	toSend += Helper::getPaddedNumber(_ans3.size(), 3);
	toSend += 3;

	toSend += Helper::getPaddedNumber(_ans4.size(), 3);
	toSend += 4;
	return toSend;
}