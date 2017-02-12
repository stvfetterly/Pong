#include "stdafx.h"
#include "Laser.h"
#include "ServiceLocator.h"
#include "Paddle.h"
#include "Game.h"


Laser::Laser(bool firingUp, const std::string& name): _name(name)
{
	//Load default image
	Load("images/Laser.png");

	assert(IsLoaded());		//The image should be loaded or we've got a problem

							//Figure out what the center of the paddle is, and use the X and Y of this position as the origin
	GetSprite().setOrigin(GetSprite().getGlobalBounds().width / 2,
		GetSprite().getGlobalBounds().height / 2);

	if (firingUp)
	{
		_velocity = -300.0;
	}
	else
	{
		_velocity = 300.0;
	}
}

Laser::~Laser()
{

}

void Laser::Draw(sf::RenderWindow& rw)
{
	VisibleGameObject::Draw(rw);
}

void Laser::Update(const float& elapsedTime)
{
	//Check if a collision with the opponent paddle has occured
	Paddle* paddle;

	//If we're moving up, then we should be checking for paddle 2 collision
	if (_velocity < 0)
	{
		paddle = dynamic_cast<Paddle*>(Game::GetGameObjectManager().Get("Paddle2"));
	}
	else //Otherwise we're going down looking for paddle 1 collision
	{
		paddle = dynamic_cast<Paddle*>(Game::GetGameObjectManager().Get("Paddle1"));
	}

	//If the paddle was found
	if (paddle != NULL)
	{
		sf::Rect<float> PaddleRect = paddle->GetBoundingRect();
		sf::Rect<float> LaserRect = GetBoundingRect();

		//Check if the laser has hit the paddle
		if (PaddleRect.intersects(LaserRect))
		{
			//If the paddle has been hit, set it's velocity to zero
			paddle->Stun();

			//hide the laser
			SetPosition(Game::SCREEN_WIDTH * 2, Game::SCREEN_HEIGHT * 2);
			return;
		}
	}
	else
	{
		//TODO: Error handling, we should have found a paddle by now
	}


	//If we've moved out of the screen, then hide the laser
	if (GetPosition().y > ( Game::SCREEN_HEIGHT + ( GetHeight() / 2 ) )  ||
		GetPosition().y < 0.0 - (GetHeight() / 2)						)
	{
		SetPosition(Game::SCREEN_WIDTH * 2, Game::SCREEN_HEIGHT * 2);
		return;
	}

	//Update the location of the laser
	GetSprite().move(0, _velocity * elapsedTime);
}