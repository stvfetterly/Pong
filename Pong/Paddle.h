#pragma once
#include "VisibleGameObject.h"

class Paddle : public VisibleGameObject
{
public:
	enum PaddleType { Manual, Auto };
	
	Paddle(PaddleType type);
	~Paddle();

	void Update(const float& elapsedTime);
	void Draw(sf::RenderWindow& rw);

	float GetVelocity() const;

private:
	float _velocity;      //positive = right, negative = left
	float _maxVelocity;
	PaddleType _type;

	void UpdateManual(const float& elapsedTime);
	void UpdateAuto(const float& elapsedTime);
};