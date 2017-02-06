#pragma once
#include "VisibleGameObject.h"

class GameBall : public VisibleGameObject
{
public:
	GameBall();
	virtual ~GameBall();

	void Update(const float& elapsedTime);

private:
	float _elapsedTimeSinceStart;
	float _angle;
	float _velocity;

	float LinearVelocityX(float angle);
	float LinearVelocityY(float angle);
};