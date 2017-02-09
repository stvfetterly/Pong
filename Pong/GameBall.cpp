#include "stdafx.h"
#include "GameBall.h"
#include "Game.h"
#include "ServiceLocator.h"
#include "Paddle.h"
#include "Score.h"

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
	
	
	//COLLISION WITH LOWER PADDLE

	//Get the two paddles
	Paddle* paddle[Game::NUM_PADDLES];

	paddle[0] = dynamic_cast<Paddle*>(Game::GetGameObjectManager().Get("Paddle1"));
	paddle[1] = dynamic_cast<Paddle*>(Game::GetGameObjectManager().Get("Paddle2"));

	//Execute collision code for each paddle
	for (int i = 0; i < Game::NUM_PADDLES; i++)
	{
		if (paddle[i] != NULL)
		{
			sf::Rect<float> p1Paddle = paddle[i]->GetBoundingRect();
			sf::Rect<float> ballRect = GetBoundingRect();

			//Check if a collision has happened between the ball and paddle
			if (p1Paddle.intersects(GetBoundingRect()))
			{
				//Riccochet off the paddle
				_angle = Game::MAX_DEGREES - _angle;

				moveByY = -moveByY;

				// Since ball is inside paddle, force it to the nearest top or bottom boundry
				if (GetPosition().y < paddle[i]->GetPosition().y)
				{
					SetPosition(GetPosition().x, paddle[i]->GetBoundingRect().top - (GetHeight() / 2));
				}
				else if (GetPosition().y >= paddle[i]->GetPosition().y)
				{
					SetPosition(GetPosition().x, paddle[i]->GetBoundingRect().top + paddle[i]->GetWidth() + (GetHeight() / 2));
				}

				// Now add "English" based on the players velocity.  
				float playerVelocity = paddle[i]->GetVelocity();

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
	}
	

	//BALL FALLS OUT TOP OR BOTTOM OF SCREEN
	if ( (GetPosition().y + GetHeight() / 2 + moveByY >= Game::SCREEN_HEIGHT ) ||	//Falling out bottom
		 (GetPosition().y + GetHeight() / 2 + moveByY < 0.0)				 )		//Falling out top
	{
		//Record if the ball has gone out the top or the bottom
		bool b_outTop = (GetPosition().y + GetHeight() / 2 + moveByY < 0.0);

		//UPDATE SCORE FOR EACH PADDLE
		Score* score[Score::MAX_SCORE];

		score[0] = dynamic_cast<Score*>(Game::GetGameObjectManager().Get("Score1"));
		score[1] = dynamic_cast<Score*>(Game::GetGameObjectManager().Get("Score2"));

		for (int i = 0; i < Score::MAX_SCORE; i++)
		{
			if ( (!b_outTop && i == 1)  ||	//If we've gone out the bottom, update score 2
				 ( b_outTop && i == 0)  )	//if we've gone out the top, update score 1
			{
				//If we're already at the max number of points, reset both scores
				if (score[i]->GetScore() == Score::MAX_SCORE)
				{
					//reset both scores
					score[0]->SetScore(0);
					score[1]->SetScore(0);
					ServiceLocator::GetAudio()->PlaySound("sounds/CheerBig3.wav");
				}
				else
				{
					//Increment score
					score[i]->SetScore(score[i]->GetScore() + 1);
				}
			}
		}

		// move to middle of the screen and randomize angle
		GetSprite().setPosition(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2);
			
		srand(static_cast<unsigned int>(time(NULL)));
		_angle = static_cast<float>(rand() % Game::MAX_DEGREES);		//Generate a random number from 0 - 359 degrees for ball angle
		_velocity = 220.0;

		//Play associated sound
		if (b_outTop)
		{
			ServiceLocator::GetAudio()->PlaySound("sounds/CheerBig.wav");		//Top Sound
		}
		else
		{
			ServiceLocator::GetAudio()->PlaySound("sounds/CheerBig2.wav");		//Bottom Sound
		}
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