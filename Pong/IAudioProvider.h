#pragma once

#include "stdafx.h"

//Pure virtual class, used to allow the possibility of switching out 
//audio libraries in the future
class IAudioProvider
{
public:
	virtual ~IAudioProvider() {}

	virtual void PlaySound(const std::string& filename) = 0;
	virtual void PlaySong(const std::string& filename, bool looping) = 0;
	virtual void StopAllSounds() = 0;
	virtual void PlayMusic() = 0;

	virtual bool IsSoundPlaying() = 0;
	virtual bool IsSongPlaying() = 0;
};