#pragma once
#include "stdafx.h"

class VisibleGameObject
{
public:
	VisibleGameObject();
	virtual ~VisibleGameObject();

	virtual void Load(const std::string& filename);
	virtual void Draw(sf::RenderWindow& window);
	virtual void Update(const float& elapsedTime);
	virtual void SetPosition(const float& x, const float& y);

	virtual bool IsLoaded() const;

	virtual float GetWidth() const;
	virtual float GetHeight() const;
	virtual sf::Rect<float> GetBoundingRect() const;
	virtual sf::Vector2f GetPosition() const;

	virtual bool IsPaused() const { return _isPaused; }
	virtual void Pause(bool newVal) { _isPaused = newVal; }

protected:
	sf::Sprite& GetSprite();
	bool _isPaused;

private:
	sf::Sprite _sprite;
	sf::Texture _image;
	std::string _filename;
	bool _isLoaded;
};
