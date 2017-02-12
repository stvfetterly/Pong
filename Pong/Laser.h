#pragma once

#include "VisibleGameObject.h"

class Laser : public VisibleGameObject
{
public:
	Laser(bool firingUp, const std::string& name);
	~Laser();

	void Update(const float& elapsedTime);
	void Draw(sf::RenderWindow& rw);
	float GetVelocity() { return _velocity; }
	void SetVelocity(const float& newVelocity) { _velocity = newVelocity; }

private:
	float _velocity;      //positive = down, negative = up
	const std::string _name;
};