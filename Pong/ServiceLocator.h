#pragma once

#include "stdafx.h"
#include "IAudioProvider.h"

class ServiceLocator
{
public:
	static IAudioProvider* GetAudio()
	{
		return _audioProvider; 
	}

	static void RegisterServiceLocator(IAudioProvider *provider)
	{
		_audioProvider = provider;
	}

	//TODO:  Add registry services for other things that SFML is being used for
	//like the SFML rendering/draw stuff

private:
	static IAudioProvider* _audioProvider;
};
