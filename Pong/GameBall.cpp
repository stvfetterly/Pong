#include "stdafx.h"
#include "GameBall.h"
#include "Game.h"
#include "ServiceLocator.h"

GameBall::GameBall() : _velocity(230.0)
{
	//Load default image
	Load("images/ball.png");
	assert(IsLoaded());		//The image should be loaded or we've got a big problem

	//Sets the center of the ball
	GetSprite().setOrigin(GetSprite().getGlobalBounds().width / 2,
		GetSprite().getGlobalBounds().height / 2);

	//Seed random number generator
	srand(static_cast<unsigned int>(time(NULL)));
	_angle = static_cast<float>(rand() % Game::MAX_DEGREES);		//Generate a random number from 0 - 359 degrees for ball angle
}

GameBall::~GameBall()
{

}

void GameBall::Update(const float& elapsedTime)
{
	//figure how much in x and y we should move
	float moveAmount = _velocity * elapsedTime;
	float moveByX = LinearVelocityX(_angle) * moveAmount;
	float moveByY = LinearVelocityY(_angle) * moveAmount;

	//COLLISION WITH EITHER SIDE OF SCREEN
	if (GetPosition().x + moveByX <= 0 + GetWidth() / 2		||					//left side of screen
		GetPosition().x + (GetHeight() / 2) + moveByX >= Game::SCREEN_WIDTH)	//right side of screen
	{
		//Ricochet off the wall logic:

		/*    270
		  225 ____ 315
			 /    \
		180	|      | 0/360
			|      |
		 135 \____/ 45
			   90
		
		If we hit a wall at 45 we want to riccochet off at 135.

		360 - angle to reverse y direction
		Add 180 to reverse x direction
		if result is greater than 360, subtract 360.

		(360 - 45) + 180 - 360 = 135
		*/
		_angle = Game::MAX_DEGREES - _angle + (Game::MAX_DEGREES / 2);
		if (_angle > Game::MAX_DEGREES)
		{
			_angle -= Game::MAX_DEGREES;
		}

		
		//prevent the ball from entering situations where it bounces back and forth endlessly left to right
		if (_angle > 170.0 && _angle < 180.0)		//Shallow left bounce
		{
			_angle -= 20.0;
		}
		else if ((_angle > 180.0 && _angle < 190.0))		//Shallow left bounce
		{
			_angle += 20.0;
		}
		else if ( _angle > 350.0 )						//Shallow right bounce
		{
			_angle -= 20.0;
		}
		else if (_angle < 10.0)							//Shallow right bounce
		{
			_angle += 20.0;
		}


		//if we were going left, go right or vise-versa
		moveByX = -moveByX;

		//increase speed of the ball
		_velocity += 5.0f;

		//Play associated sound
		ServiceLocator::GetAudio()->PlaySound("sounds/Bounce.wav");
	}
	
	
	//COLLISION WITH PADDLE
	Paddle* player1 = dynamic_cast<Paddle*>(Game::GetGameObjectManager().Get("Paddle1"));
	if (player1 != NULL)
	{
		sf::Rect<float> p1Paddle = player1->GetBoundingRect();
		sf::Rect<float> ballRect = GetBoundingRect();

		//if (ballRect.)

		//Check if a collision has happened between the ball and paddle
		if (p1Paddle.intersects( GetBoundingRect() ))
		{
			//Riccochet off the paddle

			_angle = Game::MAX_DEGREES - _angle;

			moveByY = -moveByY;
			
			// If the ball is inside paddle, move it to the top
			SetPosition(GetPosition().x, player1->GetBoundingRect().top - (GetHeight() / 2)	);

			// Now add "English" based on the players velocity.  
			float playerVelocity = player1->GetVelocity();

			if (playerVelocity < 0)			// moving left
			{
				
				_angle -= 20.0f;
				if (_angle < 0)
				{
					_angle = Game::MAX_DEGREES - _angle;
				}
			}
			else if (playerVelocity > 0)	// moving right
			{
				_angle += 20.0f;
				if (_angle > Game::MAX_DEGREES)
				{
					_angle = _angle - Game::MAX_DEGREES;
				}
			}

			//increase speed of the ball
			_velocity += 5.0f;

			//Play associated sound
			ServiceLocator::GetAudio()->PlaySound("sounds/BounceHard.wav");
		}
	}
		


	//COLLISION WITH TOP
	if (GetPosition().y - GetHeight() / 2 <= 0)
	{
		_angle = Game::MAX_DEGREES - _angle;
		moveByY = -moveByY;

		//increase speed of the ball
		_velocity += 5.0f;

		//Play associated sound
		ServiceLocator::GetAudio()->PlaySound("sounds/Bounce.wav");
	}


	//BALL FALLS OUT BOTTOM OF SCREEN
	if (GetPosition().y + GetHeight() / 2 + moveByY >= Game::SCREEN_HEIGHT)
	{
		// move to middle of the screen for now and randomize angle
		GetSprite().setPosition(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2);
			
		srand(static_cast<unsigned int>(time(NULL)));
		_angle = static_cast<float>(rand() % Game::MAX_DEGREES);		//Generate a random number from 0 - 359 degrees for ball angle
		_velocity = 220.0;

		//Play associated sound
		ServiceLocator::GetAudio()->PlaySound("sounds/CheerBig.wav");
	}

	GetSprite().move(moveByX, moveByY);
}


/*
      /|
     / |
HYP /  |
   /   |  OPPOSITE
  /    |   (Y dir)
 /_0___|
 ADJACENT  (X dir)
*/
//  cos0 = ADJACENT/HYPOTENUSE
float GameBall::LinearVelocityX(float angle)
{
	return static_cast<float> (std::cos(angle * (M_PI / 180.0)));
}

// sin0 = OPPOSITE/HYPOTENUSE
float GameBall::LinearVelocityY(float angle)
{
	return static_cast<float> (std::sin(angle * (M_PI / 180.0)));
}