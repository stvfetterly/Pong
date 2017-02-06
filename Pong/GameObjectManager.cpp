#include "stdafx.h"
#include "GameObjectManager.h"
#include "Game.h"

GameObjectManager::GameObjectManager()
{
}
GameObjectManager::~GameObjectManager()
{
	for (auto itr = _gameObjects.begin(); itr != _gameObjects.end(); ++itr)
	{
		//itr.GameObjectDeallocator();
	}
	std::for_each(_gameObjects.begin(), _gameObjects.end(), GameObjectDeallocator());
}

void GameObjectManager::Add(std::string name, VisibleGameObject* gameObject)
{
	_gameObjects.insert(std::pair<std::string, VisibleGameObject*>(name, gameObject));
}

//Deletes item from map if found
void GameObjectManager::Remove(const std::string& name)
{
	std::map<std::string, VisibleGameObject*>::iterator results = _gameObjects.find(name);
	
	//If we've found the item to remove, remove it
	if (results != _gameObjects.end())
	{
		delete results->second;
		_gameObjects.erase(results);
	}
}

//Return the object associated with the given name, if not found return NULL
VisibleGameObject* GameObjectManager::Get(const std::string& name) const
{
	std::map<std::string, VisibleGameObject*>::const_iterator results = _gameObjects.find(name);
	
	//if we've failed to find the object, return NULL
	if (results == _gameObjects.end())
	{
		return NULL;
	}

	return results->second;
}

int GameObjectManager::GetObjectCount() const
{
	return _gameObjects.size();
}

//Updates the display of items being managed
void GameObjectManager::DrawAll(sf::RenderWindow& renderWindow)
{
	for (auto itr = _gameObjects.begin(); itr != _gameObjects.end(); ++itr)
	{
		itr->second->Draw(renderWindow);
	}
}

//Updates time for each item
void GameObjectManager::UpdateAll()
{
	//finds the amount of time since last frame
	float timeDelta = clock.restart().asSeconds();

	for (auto itr = _gameObjects.begin(); itr != _gameObjects.end(); ++itr)
	{
		itr->second->Update(timeDelta);
	}
}
