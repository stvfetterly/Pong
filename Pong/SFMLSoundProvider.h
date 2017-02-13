#pragma once

#include "stdafx.h"
#include "IAudioProvider.h"
#include "SoundFileCache.h"

//Implements SFML audio library
class SFMLSoundProvider : public IAudioProvider
{
public:
	SFMLSoundProvider();
	~SFMLSoundProvider();

	void PlaySound(const std::string& filename);
	void PlaySong(const std::string& filename, bool looping);
	void PlayMusic();
	void StopAllSounds();
	void StopMusic();
	void MuteSounds(bool newVal);
	
	bool IsSoundMuted() { return _muteSounds; }
	bool IsSoundPlaying();
	bool IsSongPlaying();

private:
	static const int MAX_SOUND_CHANNELS = 5;

	SoundFileCache _soundFileCache;
	sf::Sound _currentSounds[MAX_SOUND_CHANNELS];
	std::string _currentSongName;

	bool _muteSounds;
};