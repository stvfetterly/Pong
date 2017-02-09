#pragma once

#include "stdafx.h"

class SoundFileCache
{
public:
	SoundFileCache(void);
	~SoundFileCache(void);

	sf::Sound GetSound(const std::string&) const;
	sf::Music* GetSong(const std::string&) const;

private:
	void AddSound(const std::string& soundName) const;

	static std::map<std::string, sf::SoundBuffer*> _sounds;
	static std::map<std::string, sf::Music*> _music;

	struct SoundFileDeallocator
	{
		void operator()(const std::pair<std::string, sf::SoundBuffer*>& p)
		{
			delete p.second;
		}
	};

	struct MusicFileDeallocator
	{
		void operator()(const std::pair<std::string, sf::Music*>& p)
		{
			delete p.second;
		}
	};

	template <typename T>
	struct Deallocator
	{
		void operator ()(const std::pair<std::string, T>& p)
		{
			delete p.second;
		}
	};
};

class SoundNotFoundException : public std::runtime_error
{
public:
	SoundNotFoundException(std::string const& msg) : std::runtime_error(msg){}
};