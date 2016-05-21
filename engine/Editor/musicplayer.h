#pragma once

#include <string>

#include "audioengine.h"

class MusicPlayer
{
public:
	MusicPlayer() {}
	~MusicPlayer() {}

	void InitAudio() { aEngine.init(); hasInitialized = true; }
	void LoadMusic() { if (hasInitialized) { music = aEngine.loadMusic(filePath); } }

	void PlayMusic() { if (hasInitialized) { music.play(-1); } }
	void PauseMusic() { if (hasInitialized) { music.pause(); } }
	void ResumeMusic() { if (hasInitialized) { music.resume(); } }
	void StopMusic() { if (hasInitialized) { music.stop(); } }

	//Getters
	bool hasLoadedSong() { return isSongLoaded; }

	bool hasPressedPlay() { return isPlayPressed; }
	bool hasPressedPause() { return isPausePressed; }
	bool hasPressedResume() { return isResumePressed; }
	bool hasPressedStop() { return isStopPressed; }

	const std::string& getFilePath() { return filePath; }

	//Setters
	void songIsLoaded(bool trueOrFalse) { isSongLoaded = trueOrFalse; }

	void playIsPressed(bool trueOrFalse) { isPlayPressed = trueOrFalse; }
	void pauseIsPressed(bool trueOrFalse) { isPausePressed = trueOrFalse; }
	void resumeIsPressed(bool trueOrFalse) { isResumePressed = trueOrFalse; }
	void stopIsPressed(bool trueOrFalse) { isStopPressed = trueOrFalse; }

	void setFilePath(wxString newFilePath) { filePath = newFilePath; }

	bool hasInitialized = false;

private:
	//Booleans
	bool isSongLoaded;
	bool isPlayPressed;
	bool isPausePressed;
	bool isResumePressed;
	bool isStopPressed;

	std::string filePath;

	AudioEngine aEngine;
	Music music;
	SoundEffect soundFX;
};