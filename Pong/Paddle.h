#pragma once
#include "VisibleGameObject.h"

class Paddle : public VisibleGameObject
{
public:
	enum PaddleType { Manual, Auto };
	
	const static int MAX_LASERS = 3;
	const static int TOP_Y_POS = 68;
	const static int LOW_Y_POS = 700;
	const static float AUTO_FIRE_RATE;

	Paddle(PaddleType type);
	~Paddle();

	void Update(const float& elapsedTime);
	void Draw(sf::RenderWindow& rw);

	float GetVelocity() const;
	PaddleType GetType() { return _type; }
	void SetVelocity(float newVelocity) { _velocity = newVelocity; }
	void Stun();
	void FireLaser();

private:
	float _velocity;		//positive = right, negative = left
	float _maxVelocity;
	float _stun;			//keeps track of amount of time left until paddle can move again
	float _autoFire;		//keeps track of amount of time left until auto paddle will fire again

	PaddleType _type;
	int laserFired;

	int GetLaserToFire();
	void UpdateManual(const float& elapsedTime);
	void UpdateAuto(const float& elapsedTime);
};