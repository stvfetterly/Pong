#include "stdafx.h"
#include "Game.h"
#include "MainMenuScreen.h"
#include "SplashScreen.h"
#include "SFMLSoundProvider.h"
#include "ServiceLocator.h"
#include "Paddle.h"
#include "GameBall.h"
#include "Score.h"

//Initialize static variables
Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;
GameObjectManager Game::_gameObjectManager;

//Starts the pong game
void Game::Start(void)
{
	if (_gameState != Uninitialized)
		//TODO: error handling for this state - should always start as uninitialized
		return;

	//Create the sound service and save it in the register
	SFMLSoundProvider soundProvider;
	ServiceLocator::RegisterServiceLocator(&soundProvider);

	//Creates main window with 1024x768 resolution, 32 bit colour, and a title of Pong!
	_mainWindow.create(sf::VideoMode(1024, 768, 32), "Pong!");
	
	//Create score boards
	Score* score1 = new Score();
	Score* score2 = new Score();
	score1->SetPosition((1024 / 2), 740);		//Scores go in the middle of the screen below the paddles
	score2->SetPosition((1024 / 2), 28);
	_gameObjectManager.Add("Score1", score1);
	_gameObjectManager.Add("Score2", score2);

	//Creates paddles
	Paddle* player1 = new Paddle(Paddle::Manual);		//player 1 is the lower paddle
	Paddle* player2 = new Paddle(Paddle::Auto);			//player 2 is the upper paddle
	player1->SetPosition((1024 / 2), 700);				//Paddle1 in the middle, bottom
	player2->SetPosition((1024 / 2), 68);				//Paddle2 in the middle, top
	_gameObjectManager.Add("Paddle1", player1);
	_gameObjectManager.Add("Paddle2", player2);
	
	//Create the ball
	GameBall* ball = new GameBall();
	ball->SetPosition((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) - 15);
	_gameObjectManager.Add("Ball", ball);


	//Pause all game objects so they don't start moving while at the splash screen
	_gameObjectManager.SetPause(true);

	_gameState = Game::ShowingSplash;

	//Continue to loop the game until an exit command is recieved
	while (!IsExiting())
	{
		GameLoop();
	}

	_mainWindow.close();
}

//Variable is set when user wants to exit the game
bool Game::IsExiting()
{
	if (_gameState == Game::Exiting)
		return true;
	else
		return false;
}

//This is where everything happens in the game
void Game::GameLoop()
{
	sf::Event currentEvent;
	_mainWindow.pollEvent(currentEvent);  //Check if an event has occurred

	//Ensure that the game music is always playing
	ServiceLocator::GetAudio()->PlayMusic();

	//Determine what actions should take place based on state of game
	switch (_gameState)
	{
		//User is playing the game
		case Game::Playing:
			//Clears screen to black
			_mainWindow.clear(sf::Color(0, 0, 0));

			//Update the positions of objects
			_gameObjectManager.UpdateAll();

			//Update display
			_gameObjectManager.DrawAll(_mainWindow);

			//Displays the screen
			_mainWindow.display();

			//If we get an exiting event, close the game
			if (currentEvent.type == sf::Event::Closed)
			{
				_gameState = Game::Exiting;
			}

			//If the user presses esc, back out to the menu
			if (currentEvent.type == sf::Event::KeyPressed)
			{
				if (currentEvent.key.code == sf::Keyboard::Escape)
				{
					ShowMenu();
					_gameState = Game::ShowingMenu;
				}
			}

			//If the user presses space, pause the game
			if (currentEvent.type == sf::Event::KeyPressed)
			{
 				if (currentEvent.key.code == sf::Keyboard::Space)
				{
					_gameObjectManager.SetPause(!_gameObjectManager.GetPause());
				}
			}
			break;

		//show the main menu
		case Game::ShowingMenu:
			_gameObjectManager.SetPause(true);
			ShowMenu();
			break;
		//Show the splash screen
		case Game::ShowingSplash:
			_gameObjectManager.SetPause(true);
			ShowSplashScreen();
			break;
	}
}

void Game::ShowSplashScreen()
{
	SplashScreen splashScreen;
	splashScreen.Show(_mainWindow);
	_gameState = Game::ShowingMenu;
}

//Displays main menu, allows user to play game or exit
void Game::ShowMenu()
{
	MainMenu mainMenu;
	MainMenu::MenuResult result = mainMenu.Show(_mainWindow);
	switch (result)
	{
	case MainMenu::Exit:
		_gameState = Game::Exiting;
		break;
	case MainMenu::Play:
		_gameState = Game::Playing;

		//Start all the objects moving!
		_gameObjectManager.SetPause(false);
		break;
	}
}

//Get events from the game (keyboard inputs)
const sf::Event Game::GetInput()
{
	sf::Event currentEvent;
	_mainWindow.pollEvent(currentEvent);
	return currentEvent;
}

const GameObjectManager& Game::GetGameObjectManager()
{
	return Game::_gameObjectManager;
}

