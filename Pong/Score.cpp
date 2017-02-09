#include "stdafx.h"
#include "Score.h"

Score::Score():_score(0)
{
	//Load default image
	Load("images/Zero.png");

	assert(IsLoaded());		//The image should be loaded or we've got a problem

							//Figure out what the center of the paddle is, and use the X and Y of this position as the origin
	GetSprite().setOrigin(GetSprite().getGlobalBounds().width / 2,
		GetSprite().getGlobalBounds().height / 2);
}

Score::~Score()
{

}

void Score::Draw(sf::RenderWindow& rw)
{
	VisibleGameObject::Draw(rw);
}

void Score::SetScore(int value)
{
	//Score must have changed to be updated, and score must be between zero and nine
	if (_score != value && value <= MAX_SCORE && value >= 0)
	{
		_score = value;

		//Update image to display corresponding score
		switch (_score)
		{
		case 0:
			Load("images/Zero.png");
			break;
		case 1:
			Load("images/One.png");
			break;
		case 2:
			Load("images/Two.png");
			break;
		case 3:
			Load("images/Three.png");
			break;
		case 4:
			Load("images/Four.png");
			break;
		case 5:
			Load("images/Five.png");
			break;
		case 6:
			Load("images/Six.png");
			break;
		case 7:
			Load("images/Seven.png");
			break;
		case 8:
			Load("images/Eight.png");
			break;
		case 9:
			Load("images/Nine.png");
			break;
		}

		assert(IsLoaded());		//The image should be loaded or we've got a problem
	}
}