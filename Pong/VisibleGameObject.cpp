#include "stdafx.h"
#include "VisibleGameObject.h"


//Default constructor - initialize variables
VisibleGameObject::VisibleGameObject() : _isLoaded(false), _filename(""), _isPaused(true)
{

}

//Destructor
VisibleGameObject::~VisibleGameObject()
{

}

void VisibleGameObject::Load(const std::string& filename)
{
	//if we fail to load the image, reset variables
	if (_image.loadFromFile(filename) == false)
	{
		_filename = "";
		_isLoaded = false;
	}
	else //Loads the image
	{
		_filename = filename;
		_sprite.setTexture(_image, true);
		_isLoaded = true;
	}
}

void VisibleGameObject::Draw(sf::RenderWindow& renderWindow)
{
	//Draw an image if we've loaded anything
	if (_isLoaded)
	{
		renderWindow.draw(_sprite);
	}
}

void VisibleGameObject::Update(const float& elapsedTime)
{

}

void VisibleGameObject::SetPosition(const float& x, const float& y)
{
	if (_isLoaded)
	{
		_sprite.setPosition(x, y);
	}
}

//Returns the position if the object has been loaded, otherwise returns empty vector
sf::Vector2f VisibleGameObject::GetPosition() const
{
	if (_isLoaded)
	{
		return _sprite.getPosition();
	}
	return sf::Vector2f();
}

sf::Sprite& VisibleGameObject::GetSprite()
{
	return _sprite;
}

bool VisibleGameObject::IsLoaded() const
{
	return _isLoaded;
}

float VisibleGameObject::GetHeight() const
{
	return _sprite.getLocalBounds().height;
}

float VisibleGameObject::GetWidth() const
{
	return _sprite.getLocalBounds().width;
}

//Returns a rectangle that contains the object
sf::Rect<float> VisibleGameObject::GetBoundingRect() const
{
/*	sf::Vector2f size;
	size.x = GetWidth();
	size.y = GetHeight();

	sf::Vector2f centerPos;
	centerPos.x = _sprite.getOrigin().x;
	centerPos.y = _sprite.getOrigin().y;

	return sf::Rect<float>( centerPos.x - size.x/2,
							centerPos.y - size.x/2,
							centerPos.x + size.x/2,
							centerPos.y + size.y/2);
							**/

	return _sprite.getGlobalBounds();
}

