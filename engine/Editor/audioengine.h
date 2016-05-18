#pragma once

#include <string>
#include <map>
#include <MIXER\SDL_mixer.h>

class SoundEffect
{
public:
	friend class AudioEngine;

	void play(int loopTimes = 0);

private:
	Mix_Chunk *m_chunk;
};

class Music
{
public:
	friend class AudioEngine;

	void play(int loopTimes = -1);

	static void pause();
	static void stop();
	static void resume();

private:
	Mix_Music *m_music = nullptr;
};

class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();

	void init();
	void destroy();

	void destroyMusic();

	SoundEffect loadSoundEffect(const std::string &filePath);
	Music loadMusic(const std::string &filePath);

private:
	bool m_isInitialized = false;

	std::map<std::string, Mix_Chunk *> m_effectMap;
	std::map<std::string, Mix_Music *> m_musicMap;
};