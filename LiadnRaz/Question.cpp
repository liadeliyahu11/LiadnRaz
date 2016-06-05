#include "Question.h"




Question::Question(int id,string correctAns,string ans2,string ans3,string ans4)
{
	_id = id;
	_correctAns = correctAns;
	_ans2 = ans2;
	_ans3 = ans3;
	_ans4 = ans4;
}

Question::~Question()
{
	_id = nullptr;
	_correctAns = nullptr;
	_ans2 = nullptr;
	_ans3 = nullptr;
	_ans4 = nullptr;
}