#pragma once
#include "stdafx.h"

class MainMenu
{

public:
	MainMenu();

	//Options for the menu selection
	enum MenuResult
	{ 
		Nothing, 
		Play, 
			OptGun, 
			OptMusic, 
			OptSound, 
				OptDifPansy, 
				OptDifWannabe, 
				OptDifHard, 
			OptConstMotion, 
		Exit 
	};

	struct MenuItem
	{
	public:
		sf::Rect<int> rect;
		MenuResult action;
	};

	MenuResult Show(sf::RenderWindow& window);

private:
	MenuResult GetMenuResponse(sf::RenderWindow& window);
	MenuResult HandleClick(int x, int y);
	std::list<MenuItem> _menuItems;

	sf::Texture _checkedImage;
	sf::Texture _unCheckedImage;
	sf::Sprite _gunBox;
	sf::Sprite _musicBox;
	sf::Sprite _soundBox;
	sf::Sprite _pansyBox;
	sf::Sprite _wannabeBox;
	sf::Sprite _hardcoreBox;
	sf::Sprite _constantBox;

	void UpdateBox(sf::RenderWindow& window, sf::Sprite& updatedSprite, bool newVal);
};