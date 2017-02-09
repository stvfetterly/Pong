#include "stdafx.h"
#include "SoundFileCache.h"

std::map<std::string, sf::SoundBuffer*> SoundFileCache::_sounds;
std::map<std::string, sf::Music*> SoundFileCache::_music;

SoundFileCache::SoundFileCache(void)
{
	//Load commonly used sounds into cache upon creation to avoid delays during gameplay
	AddSound("sounds/Bounce.wav");
	AddSound("sounds/Bounce2.wav");
	AddSound("sounds/BounceHard.wav");
	AddSound("sounds/CheerBig.wav");
	AddSound("sounds/CheerBig2.wav");
	AddSound("sounds/CheerBig3.wav");
	AddSound("sounds/CheerSmall.wav");
	AddSound("sounds/Click.wav");
	AddSound("sounds/Inflate.wav");
}

//Delete all the stuff we've created and saved in the maps
SoundFileCache::~SoundFileCache(void)
{
	std::for_each(_sounds.begin(), _sounds.end(), Deallocator<sf::SoundBuffer*>());
	std::for_each(_music.begin(), _music.end(), Deallocator<sf::Music*>());
}

//Adds a new sound to the cache
void SoundFileCache::AddSound(const std::string& soundName) const
{
	sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();
	if (!soundBuffer->loadFromFile(soundName))
	{
		//sound file can't be found, that's a problem
		delete soundBuffer;
		throw SoundNotFoundException(soundName + " was not found in call to SoundFileCache::GetSound");
	}

	//Sound was found, add it to the map
	std::map<std::string, sf::SoundBuffer*>::iterator res =
		_sounds.insert(std::pair<std::string, sf::SoundBuffer*>(soundName, soundBuffer)).first;

	sf::Sound sound;
	sound.setBuffer(*soundBuffer);
}

sf::Sound SoundFileCache::GetSound(const std::string& soundName) const
{
	std::map<std::string, sf::SoundBuffer*>::iterator itr = _sounds.find(soundName);

	//Check if the sound is already in the map.  If it is missing we add it to the map and return it.
	if (itr == _sounds.end())
	{
		AddSound(soundName);

		//Find the newly added sound
		itr = _sounds.find(soundName);

		//Return it
		sf::Sound sound;
		sound.setBuffer(*itr->second);
		return sound;
	}
	else    //return the sound
	{
		sf::Sound sound;
		sound.setBuffer(*itr->second);
		return sound;
	}

	//Should never get here
	throw SoundNotFoundException(soundName + " was not found in call to SoundFileCache::GetSound");
}

sf::Music* SoundFileCache::GetSong(const std::string& soundName) const
{
	std::map<std::string, sf::Music*>::iterator itr = _music.find(soundName);

	//Check if the song is already in the map.  If it is, we return it.  Otherwise we add it to the map and return it.
	if (itr == _music.end())
	{
		sf::Music* song = new sf::Music();
		if (!song->openFromFile(soundName))
		{
			//If the song can't be found, we have a problem
			delete song;
			throw SoundNotFoundException(soundName + " was not found in call to SoundFileCache::GetSong");
		}
		else
		{
			//Song found, add it to the music map
			std::map<std::string, sf::Music*>::iterator res =
				_music.insert(std::pair<std::string, sf::Music*>(soundName, song)).first;
			return res->second;
		}
	}
	else
	{
		return itr->second;
	}

	//Should never get here
	throw SoundNotFoundException(soundName + " was not found in call to SoundFileCache::GetSong");
}