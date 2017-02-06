#include "StdAfx.h"
#include "SplashScreen.h"


void SplashScreen::Show(sf::RenderWindow & renderWindow)
{
	//Create a variable to hold the splash screen image
	sf::Texture image;
	if (image.loadFromFile("images/Splash.png") != true)
	{
		//If the image is not found, just return
		//TODO: Error handling
		return;
	}

	//Draw/display the image to user
	sf::Sprite sprite(image);
	renderWindow.draw(sprite);
	renderWindow.display();

	//When the user to presses a key, clicks the mouse, or closes the window we generate an event
	sf::Event event;
	while (true)
	{
		while (renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::KeyPressed
				|| event.type == sf::Event::EventType::MouseButtonPressed
				|| event.type == sf::Event::EventType::Closed)
			{
				return;    
			}
		}
	}
}