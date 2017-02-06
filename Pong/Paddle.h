#pragma once
#include "VisibleGameObject.h"

class Paddle : public VisibleGameObject
{
public:
	Paddle();
	~Paddle();

	void Update(const float& elapsedTime);
	void Draw(sf::RenderWindow& rw);

	float GetVelocity() const;

private:
	float _velocity;      //positive = right, negative = left
	float _maxVelocity;
};