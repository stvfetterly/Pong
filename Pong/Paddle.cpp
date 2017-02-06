#include "stdafx.h"
#include "Paddle.h"
#include "Game.h"

Paddle::Paddle(): _velocity(0), _maxVelocity(600.0)
{
	//Load default image
	Load("images/paddle.png");

	assert(IsLoaded());		//The image should be loaded or we've got a problem

	//Figure out what the center of the paddle is, and use the X and Y of this position as the origin
	GetSprite().setOrigin(GetSprite().getGlobalBounds().width / 2,
		GetSprite().getGlobalBounds().height/2);
}

Paddle::~Paddle()
{

}

void Paddle::Draw(sf::RenderWindow& rw)
{
	VisibleGameObject::Draw(rw);
}

float Paddle::GetVelocity() const
{
	return _velocity;
}

void Paddle::Update(const float& elapsedTime)
{

	//Constant motion code

	//Left makes the paddle move left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		_velocity -= 3.0;
	}

	//Right makes the paddle move right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		_velocity += 3.0;
	}

	//Down makes the paddle stop moving
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		_velocity = 0.0;
	}

	//Ensure that paddle speed stays within speed limits
	if (_velocity > _maxVelocity)
	{
		_velocity = _maxVelocity;
	}
	else if (_velocity < -_maxVelocity)
	{
		_velocity = -_maxVelocity;
	}

	sf::Vector2f pos = this->GetPosition();

	//Paddle will bounce off the wall if it gets to the left side
	float leftBound = (GetSprite().getLocalBounds().width / 2);
	float rightBound = (Game::SCREEN_WIDTH - GetSprite().getLocalBounds().width / 2);

	if (pos.x < leftBound)
	{
		this->SetPosition(leftBound, pos.y);
		_velocity = -_velocity;
	}
	//Paddle will bounce off the right side wall too
	else if (pos.x > rightBound)
	{
		this->SetPosition(rightBound, pos.y);
		_velocity = -_velocity;
	}
	else
	{
		//Move the location of the paddle
		GetSprite().move(_velocity * elapsedTime, 0);
	}




/*
	//Moves only when user presses keys
	float move = 0.0;

	//Left makes the paddle move left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		move -= 1000.0 * elapsedTime;
	}

	//Right makes the paddle move right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		move += 1000.0 * elapsedTime;
	}

	sf::Vector2f pos = this->GetPosition();

	//Do not move further than the left wall
	if (pos.x + move < (GetSprite().getLocalBounds().width / 2))
	{
		this->SetPosition((GetSprite().getLocalBounds().width / 2), pos.y);
	}
	//Do not move further than the right wall
	else if (pos.x + move >(Game::SCREEN_WIDTH - GetSprite().getLocalBounds().width / 2))
	{
		this->SetPosition(Game::SCREEN_WIDTH - GetSprite().getLocalBounds().width / 2, pos.y);
	}
	else
	{
		this->SetPosition(pos.x + move, pos.y);
	}
*/

}