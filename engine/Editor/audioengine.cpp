#include "audioengine.h"
#include "commonerrors.h"

void SoundEffect::play(int loopTimes)
{
	if (Mix_PlayChannel(-1, m_chunk, loopTimes) == -1)
	{
		if (Mix_PlayChannel(0, m_chunk, loopTimes) == -1)
		{
			fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
		}
	}
}

void Music::play(int loopTimes)
{
	if (Mix_PlayMusic(m_music, loopTimes) == -1)
	{
		fatalError("Mix_PlayMusic error: " + std::string(Mix_GetError()));
	}
}

void Music::pause()
{
	Mix_PauseMusic();
}

void Music::stop()
{
	Mix_HaltMusic();
}

void Music::resume()
{
	Mix_ResumeMusic();
}

AudioEngine::AudioEngine()
{
}

AudioEngine::~AudioEngine()
{
	destroy();
}

void AudioEngine::init()
{
	if (m_isInitialized == true)
	{
		fatalError("Tried to initialize audio engine more than once.");
	}

	//Parameter can be a bitwise combination of MIX_INIT_FAC,
	//MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1)
	{
		fatalError("Mix_Init error: " + std::string(Mix_GetError()));
	}

	if (Mix_OpenAudio(/*MIX_DEFAULT_FREQUENCY*/44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		fatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));
	}

	m_isInitialized = true;
}

void AudioEngine::destroy()
{
	if (m_isInitialized == true)
	{
		for (auto &it : m_effectMap)
		{
			Mix_FreeChunk(it.second);
		}

		for (auto &it : m_musicMap)
		{
			Mix_FreeMusic(it.second);
		}

		m_effectMap.clear();
		m_musicMap.clear();

		Mix_CloseAudio();
		Mix_Quit();

		m_isInitialized = false;
	}
}

void AudioEngine::destroyMusic()
{
	if (m_isInitialized == true)
	{
		for (auto &it : m_musicMap)
		{
			Mix_FreeMusic(it.second);
		}

		m_musicMap.clear();

		Mix_CloseAudio();
		Mix_Quit();

		m_isInitialized = false;
	}
}

SoundEffect AudioEngine::loadSoundEffect(const std::string &filePath)
{
	//Try to find the chunk in the cache
	auto it = m_effectMap.find(filePath);

	SoundEffect effect;

	if (it == m_effectMap.end())
	{
		//Failed to find chunk, must load
		Mix_Chunk *chunk = Mix_LoadWAV(filePath.c_str());
		
		//Check for errors
		if (chunk == nullptr)
		{
			fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
		}			
		else if (chunk != nullptr)
		{
			printf("Successfully loaded file!\n");
		}

		effect.m_chunk = chunk;

		if (effect.m_chunk != nullptr)
		{
			printf("m_chunk is not null!\n");
		}

		m_effectMap[filePath] = chunk;
	}
	else
	{
		//It's already cached
		effect.m_chunk = it->second;
	}

	return effect;
}

Music AudioEngine::loadMusic(const std::string &filePath)
{
	//Try to find the chunk in the cache
	auto it = m_musicMap.find(filePath);

	Music music;

	if (it == m_musicMap.end())
	{
		//Failed to find chunk, must load
		Mix_Music *mixMusic = Mix_LoadMUS(filePath.c_str());

		//Check for errors
		if (mixMusic == nullptr)
		{
			fatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
		}

		music.m_music = mixMusic;

		m_musicMap[filePath] = mixMusic;
	}
	else
	{
		//It's already cached
		music.m_music = it->second;
	}

	return music;
}