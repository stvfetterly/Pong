#include "stdafx.h"
#include "Paddle.h"
#include "Game.h"
#include "ServiceLocator.h"
#include "GameBall.h"
#include "Laser.h"

const float Paddle::AUTO_FIRE_RATE = 0.6;

Paddle::Paddle(Paddle::PaddleType type) : _velocity(0), _maxVelocity(600.0), _type(type), 
										  _stun(0.0), laserFired(0), _autoFire(AUTO_FIRE_RATE)
{
	//Load default image
	Load("images/paddle.png");

	assert(IsLoaded());		//The image should be loaded or we've got a problem

							//Figure out what the center of the paddle is, and use the X and Y of this position as the origin
	GetSprite().setOrigin(GetSprite().getGlobalBounds().width / 2,
		GetSprite().getGlobalBounds().height / 2);
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
	//If the paddle is stunned, do not move until time has elapsed
	if (_stun > 0)
	{
		_stun -= elapsedTime;
	}
	else
	{
		//Update the location and firing of automatic paddle
		if (_type == Paddle::Auto)
		{
			_autoFire -= elapsedTime;
			if (_autoFire < 0)
			{
				_autoFire = AUTO_FIRE_RATE;
				FireLaser();
			}

			UpdateAuto(elapsedTime);
		}
		else   //Update location of manual paddle
		{
			UpdateManual(elapsedTime);
		}
	}
}

void Paddle::UpdateAuto(const float& elapsedTime)
{
	const GameBall* gameBall = dynamic_cast<GameBall*> (Game::GetGameObjectManager().Get("Ball"));

	if (NULL != gameBall)
	{
		sf::Vector2f ballPosition = gameBall->GetPosition();

		//ignore ball position if it's going to hit the paddle
		if (std::abs(GetPosition().x - ballPosition.x) > (this->GetWidth() / 4))
		{
			//if the ball is further right than the paddle, move towards it
			if (GetPosition().x < ballPosition.x)
			{
				_velocity += 1.0;
			}
			//ditto for the left
			else if (GetPosition().x > ballPosition.x)
			{
				_velocity -= 1.0;
			}
		}

		//Limit the paddle to max velocity
		if (_velocity > _maxVelocity)
		{
			_velocity = _maxVelocity;
		}
		else if (_velocity < -_maxVelocity)
		{
			_velocity = -_maxVelocity;
		}

		sf::Vector2f pos = this->GetPosition();

		//Check if we've gone past the right of the screen
		if (GetPosition().x >(Game::SCREEN_WIDTH - (GetWidth() / 2)))
		{
			//if so, place the paddle in the right most position possible
			SetPosition((Game::SCREEN_WIDTH - (GetWidth() / 2)), GetPosition().y);
			_velocity = -_velocity;	//bounce

									//Play associated sound
			ServiceLocator::GetAudio()->PlaySound("sounds/Bounce2.wav");
		}
		//Check if we're past the left of the screen
		else if (GetPosition().x < 0.0 + (GetWidth() / 2))
		{
			//if so, place the paddle in the left most position possible
			SetPosition(0.0f + (GetWidth() / 2), GetPosition().y);
			_velocity = -_velocity;	//bounce

									//Play associated sound
			ServiceLocator::GetAudio()->PlaySound("sounds/Bounce2.wav");
		}

		//Move the paddle!
		GetSprite().move(_velocity*elapsedTime, 0);
	}
	else
	{
		//TODO - this is a big error . . . there's no ball!
	}
}

void Paddle::UpdateManual(const float& elapsedTime)
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

		//Play associated sound
		ServiceLocator::GetAudio()->PlaySound("sounds/Bounce2.wav");
	}
	//Paddle will bounce off the right side wall too
	else if (pos.x > rightBound)
	{
		this->SetPosition(rightBound, pos.y);
		_velocity = -_velocity;

		//Play associated sound
		ServiceLocator::GetAudio()->PlaySound("sounds/Bounce2.wav");
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

void Paddle::Stun()
{
	//prevents paddle movement for a second
	_stun = 1.0;

	//Plays sound
	ServiceLocator::GetAudio()->PlaySound("sounds/Boom.wav");
}

int Paddle::GetLaserToFire()
{
	//return the next available laser to be fired
	int retVal = laserFired;

	//Update the laser fired val
	laserFired++;
	if (laserFired == MAX_LASERS)
	{
		laserFired = 0;
	}

	return retVal;
}

void Paddle::FireLaser()
{
	Laser* firingLaser;

	//Find next free laser to shoot
	if (GetType() == Paddle::Auto)
	{
		switch (GetLaserToFire())
		{
		case 0:
			firingLaser = dynamic_cast<Laser*>(Game::GetGameObjectManager().Get("Laser"));
			break;
		case 1:
			firingLaser = dynamic_cast<Laser*>(Game::GetGameObjectManager().Get("Laser1"));
			break;
		case 2:
			firingLaser = dynamic_cast<Laser*>(Game::GetGameObjectManager().Get("Laser2"));
			break;
		}
	}
	else  //Manual firing laser
	{
		switch (GetLaserToFire())
		{
		case 0:
			firingLaser = dynamic_cast<Laser*>(Game::GetGameObjectManager().Get("Laser3"));
			break;
		case 1:
			firingLaser = dynamic_cast<Laser*>(Game::GetGameObjectManager().Get("Laser4"));
			break;
		case 2:
			firingLaser = dynamic_cast<Laser*>(Game::GetGameObjectManager().Get("Laser5"));
			break;
		}
	}


	if (firingLaser != NULL)
	{
		//Get center of the paddle as x location
		float xPos = GetPosition().x;
		float yPos = GetPosition().y;

		//Get either the top or bottom of the paddle as y location
		if (yPos == Paddle::TOP_Y_POS)
		{
			//Get Top
			yPos = yPos + firingLaser->GetHeight();
			firingLaser->SetVelocity( abs( firingLaser->GetVelocity() ) );		//laser must go down, so velocity must be negative
		}
		else
		{
			//Get Bottom
			yPos = yPos - firingLaser->GetHeight();
			firingLaser->SetVelocity( -abs( firingLaser->GetVelocity() ) );		//laser must go up, so velocity must be positive
		}

		firingLaser->SetPosition(xPos, yPos);

		//Laser sound
		ServiceLocator::GetAudio()->PlaySound("sounds/LaserBlast.wav");
	}
	else
	{
		//TODO - Error handling, laser not found!
	}
}