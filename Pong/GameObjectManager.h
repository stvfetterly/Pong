#pragma once
#include "VisibleGameObject.h"

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	void Add(std::string name, VisibleGameObject* gameObject);
	void Remove(const std::string& name);
	int GetObjectCount() const;
	VisibleGameObject* Get(const std::string& name) const;
	
	void DrawAll(sf::RenderWindow& renderWindow);
	void UpdateAll();
	void SetPause(bool pause);
	bool GetPause() { return _paused; }

private:
	std::map<std::string, VisibleGameObject*> _gameObjects;
	sf::Clock clock;

	bool _paused;

	struct GameObjectDeallocator
	{
		void operator()(const std::pair<std::string, VisibleGameObject*>& p)
		{
			delete p.second;
		}
	};
};
