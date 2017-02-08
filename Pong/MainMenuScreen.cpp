#include "stdafx.h"
#include "MainMenuScreen.h"
#include "ServiceLocator.h"

//Show the main menu image, set up the clickable coordinates for option selection
MainMenu::MenuResult MainMenu::Show(sf::RenderWindow& window)
{

	//Load menu image from file
	sf::Texture image;
	image.loadFromFile("images/mainmenu.png");
	sf::Sprite sprite(image);

	//Setup clickable regions

	//Play menu item coordinates
	MenuItem playButton;
	playButton.rect.top = 145;			//top
	playButton.rect.height = 380;		//bottom
	playButton.rect.left = 0;			//left
	playButton.rect.width = 1023;		//right
	playButton.action = Play;

	//Exit menu item coordinates
	MenuItem exitButton;
	exitButton.rect.left = 0;			//left
	exitButton.rect.width = 1023;		//right
	exitButton.rect.top = 383;			//top
	exitButton.rect.height = 560;		//bottom
	exitButton.action = Exit;

	_menuItems.push_back(playButton);
	_menuItems.push_back(exitButton);

	window.draw(sprite);
	window.display();

	return GetMenuResponse(window);
}

//
MainMenu::MenuResult MainMenu::HandleClick(int x, int y)
{
	std::list<MenuItem>::iterator it;

	//Check if one of the menu items has been clicked
	for (it = _menuItems.begin(); it != _menuItems.end(); it++)
	{
		sf::Rect<int> menuItemRect = (*it).rect;
		if (menuItemRect.height > y			//bottom
			&& menuItemRect.top < y			//top
			&& menuItemRect.left < x		//left
			&& menuItemRect.width > x)		//right
		{
			//Play selection sound
			ServiceLocator::GetAudio()->PlaySound("sounds/Click.wav");

			return (*it).action;
		}
	}

	return Nothing;
}

MainMenu::MenuResult  MainMenu::GetMenuResponse(sf::RenderWindow& window)
{
	sf::Event menuEvent;

	while (true)
	{
		//Loop forever, checking for a mouse button pressed or window closed event
		while (window.pollEvent(menuEvent))
		{
			if (menuEvent.type == sf::Event::MouseButtonPressed)
			{
				return HandleClick(menuEvent.mouseButton.x, menuEvent.mouseButton.y);
			}
			if (menuEvent.type == sf::Event::Closed)
			{
				return Exit;
			}
		}
	}
}