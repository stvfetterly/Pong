#pragma once
#include "VisibleGameObject.h"

class Score : public VisibleGameObject
{
public:
	Score();
	~Score();

	static const int MAX_SCORE = 9;

	void Draw(sf::RenderWindow& rw);

	int GetScore(){return _score;}
	void SetScore(int value);

private:
	int _score;
};