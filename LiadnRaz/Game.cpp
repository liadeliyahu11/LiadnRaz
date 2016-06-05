#include "Game.h"

Game::Game(const vector<User *>& players, int questionNo, DataBase &db)
{
	for (unsigned int i = 0; i < players.size();i++)
	{
		_players.push_back(players[i]);
		_players[i]->setGame(this);
		_results[_players[i]->getUsername()] = 0;
	}
	_questionNo = questionNo;
	_db = new DataBase(db);
	_db->insertNewGame();//need to check raz didnt do.
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