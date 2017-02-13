#pragma once

#include "stdafx.h"

//Pure virtual class, used to allow the possibility of switching out 
//audio libraries in the future
class IAudioProvider
{
public:
	virtual ~IAudioProvider() {}

	virtual void PlaySound(const std::string& filename) = 0;				//Plays an individual sound
	virtual void PlaySong(const std::string& filename, bool looping) = 0;	//Plays an individual song
	virtual void PlayMusic() = 0;											//Plays multiple songs

	virtual void StopAllSounds() = 0;										//Stops all music and sounds currently playing
	virtual void StopMusic() = 0;											//Stops currently playing music
	virtual void MuteSounds(bool newVal) = 0;								//turns sounds on or off for the game
	
	virtual bool IsSoundMuted() = 0;
	virtual bool IsSoundPlaying() = 0;
	virtual bool IsSongPlaying() = 0;
};