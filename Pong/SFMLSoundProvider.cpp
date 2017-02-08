#include "stdafx.h"
#include "SFMLSoundProvider.h"
#include "SoundFileCache.h"

sf::SoundBuffer _soundBuffer;
sf::Sound _sound;
sf::Music _music;

SFMLSoundProvider::SFMLSoundProvider()
{
	//Default values so that sounds are heard over the music
	_sound.setVolume(100.0);
	_music.setVolume(80.0);
}

SFMLSoundProvider::~SFMLSoundProvider()
{

}

void SFMLSoundProvider::PlaySound(const std::string& filename)
{
	int availChannel = -1;

	for (int i = 0; i < MAX_SOUND_CHANNELS; i++)
	{
		//Find a free channel to play on
		if (_currentSounds[i].getStatus() != sf::Sound::Playing)
		{
			availChannel = i;
			break;
		}
	}

	//if we have an available channel, then play sound
	if (availChannel != -1)
	{
		try
		{
			_currentSounds[availChannel] = _soundFileCache.GetSound(filename);
			_currentSounds[availChannel].play();
		}
		catch (SoundNotFoundException&)
		{
			//ERROR, file not found
			//TODO - ERROR HANDLING
		}
	}
}

//Randomly selects a song from the available music files and plays it
void SFMLSoundProvider::PlayMusic()
{
	// Check if a song is playing already
	if (_currentSongName != "")
	{
		try
		{
			//If the song is already playing, then let it continue to play
			sf::Music* currentSong = _soundFileCache.GetSong(_currentSongName);
			if (currentSong->getStatus() != sf::Sound::Stopped)
			{
				return;
			}
		}
		catch (SoundNotFoundException&)
		{
			//TODO handle error - previous song we're trying to stop wasn't located
			return;
		}
	}

	//if the selected song isn't playing, or if no song has been selected then randomly grab a new one
	const int NUM_SONGS = 4;

	std::string songArray[NUM_SONGS];
	songArray[0] = "sounds/Music.ogg";
	songArray[1] = "sounds/Music1.ogg";
	songArray[2] = "sounds/Music2.ogg";
	songArray[3] = "sounds/Music3.ogg";

	//Seed random number generator and generate a random number
	srand(static_cast<unsigned int>(time(NULL)));
	unsigned int i = rand() % NUM_SONGS;

	PlaySong(songArray[i], false);
}

void SFMLSoundProvider::PlaySong(const std::string& filename, bool looping)
{
	sf::Music * currentSong;

	try
	{
		currentSong = _soundFileCache.GetSong(filename);
	}
	catch (SoundNotFoundException&)
	{
		//TODO handle error - couldn't find or load song
		return;
	}

	// Check if a song is playing already
	if (_currentSongName != "")
	{
		try
		{
			sf::Music* priorSong = _soundFileCache.GetSong(_currentSongName);
			if (priorSong->getStatus() != sf::Sound::Stopped)
			{
				priorSong->stop();
			}
		}
		catch (SoundNotFoundException&)
		{
			//TODO handle error - previous song we're trying to stop wasn't located
		}
	}

	_currentSongName = filename;
	currentSong->setLoop(looping);
	currentSong->play();
}

void SFMLSoundProvider::StopAllSounds()
{
	//Kill sounds
	for (int i = 0; i < MAX_SOUND_CHANNELS; i++)
	{
		_currentSounds[i].stop();
	}

	//Kill music
	if (_currentSongName != "")
	{
		sf::Music* currentSong = _soundFileCache.GetSong(_currentSongName);
		if (currentSong->getStatus() == sf::Sound::Playing)
		{
			currentSong->stop();
		}
	}
}

bool SFMLSoundProvider::IsSoundPlaying()
{
	//loop through all the audio channels, if any of them are playing a sound then return true
	for (int i = 0; i < MAX_SOUND_CHANNELS; i++)
	{
		if (_currentSounds[i].getStatus() == sf::Sound::Playing)
		{
			return true;
		}
	}
	return false;
}

bool SFMLSoundProvider::IsSongPlaying()
{
	//Check if we're currently playing a song
	if (_currentSongName != "")
	{
		return _soundFileCache.GetSong(_currentSongName)->getStatus() == sf::Music::Playing;
	}
	return false;
}