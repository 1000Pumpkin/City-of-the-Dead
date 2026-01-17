#pragma once

class Sound
{
private:
	int soundIndex = 0;
	int curSound = 0;
	int szSound = 0;

public:
	Sound() {};
	Sound(std::wstring name, int szSound);
	~Sound();
	
	void Play(bool loop);
	void Stop();
	void SetVolume(int volume);
};