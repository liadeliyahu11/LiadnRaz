#include "Game.h"

Game::Game(vector<User *>& players, int questionNo, DataBase * db)
{
	_questionCount = 0;
	for (unsigned int i = 0; i < players.size();i++)
	{
		_players.push_back(players[i]);
		_players[i]->setGame(this);
		_results[_players[i]->getUsername()] = 0;
	}
	_questionNo = questionNo;
	_db = new DataBase(*db);
	_id = _db->insertNewGame();//need to check raz didnt do.
	_questions = _db->initQuestions(_questionNo);//check

}

Game::~Game()
{
	for (unsigned int i = 0; i < _questions.size(); i++)
	{
		delete _questions[i];
	}
	_questions.clear();
	_players.clear();
}

void Game::sendQuestionToAllUsers()
{
	_currentTurnAnswers = 0;
	Question * curr = _questions[_questionCount];
	string msg = curr->getMessageToSend();
	for (unsigned int i = 0; i < _players.size();i++)
	{
		try
		{
			_players[i]->send(msg);
		}
		catch (...)
		{

		}
	}
}

bool Game::handleNextTurn()
{
	bool active = true;
	if (_players.size() >0)
	{
		if (_currentTurnAnswers >= _players.size())//if user quit + 1 spare 
		{
			if (_questionCount + 1 == _questionNo)//last one
			{
				handleFinishGame();
				active = false;
			}
			else
			{
				_questionCount++;
				sendQuestionToAllUsers();
			}
		}
	}
	else
	{
		active = false;
		//handleFinishGame();
	}
	return active;
}

bool Game::handleAnswerFromUser(User * user,int ansNo,int time)
{
	bool active = true,isCorrect = false;
	_currentTurnAnswers++;
	int correct = _questions[_questionCount]->getCorrect();
	if (ansNo == correct)
	{
		isCorrect = true;
		_results[user->getUsername()]++;
		user->send("1201");
	}
	else
	{
		user->send("1200");
	}
	_db->addAnswerToPlayer(_id, user->getUsername(), _questions[_questionCount]->getId(),
		_questions[_questionCount]->getAns(), isCorrect, time);
	handleNextTurn();
	return active;
}

void Game::handleFinishGame()
{
	_db->updateGameStatus(_id);
	for (unsigned int i = 0; i < _players.size();i++)
	{
		try
		{
			_players[i]->send("121");
			_players[i]->setGame(nullptr);
		}
		catch (...)
		{
			//pass
		}
	}
}

bool Game::leaveGame(User * currUser)
{
	bool retVal = false;
	for (int i = 0; i<_players.size(); i++)
	{
		if (_players[i] == currUser)
		{
			_players.erase(_players.begin() + i);
		}
	}
	if (handleNextTurn())
	{
		retVal = true;
	}
	return retVal;
}

void Game::sendFirstQuestion()
{
	sendQuestionToAllUsers();
}