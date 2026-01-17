#include "Core.h"

Sound::Sound(std::wstring name, int szSound)
{
	static int index = 0;
	soundIndex = ++index;
	for (int i = 0; i < szSound; ++i)
	{
		WCHAR buffer[255];
		swprintf_s(buffer, L"open %s alias %d_%d", name.c_str(), soundIndex, i);
		mciSendString(buffer, 0, 0, 0);
		swprintf_s(buffer, L"play %d_%d from 0", soundIndex, i);
		mciSendString(buffer, 0, 0, 0);
		swprintf_s(buffer, L"pause %d_%d", soundIndex, i);
		mciSendString(buffer, 0, 0, 0);
	}
	this->szSound = szSound;
}

Sound::~Sound()
{
	for (int i = 0; i < szSound; ++i)
	{
		WCHAR buffer[255];
		swprintf_s(buffer, L"close %d_%d", soundIndex, i);
		mciSendString(buffer, 0, 0, 0);
	}
}

void Sound::Play(bool loop)
{
	WCHAR buffer[255];
	swprintf_s(buffer, L"play %d_%d from 0%s", soundIndex, curSound, loop ? L" repeat" : L"");
	mciSendString(buffer, 0, 0, 0);
	if (++curSound >= szSound)
		curSound = 0;
}

void Sound::Stop()
{
	for (int i = 0; i < szSound; ++i)
	{
		WCHAR buffer[255];
		swprintf_s(buffer, L"stop %d_%d", soundIndex, i);
		mciSendString(buffer, 0, 0, 0);
	}
}

void Sound::SetVolume(int volume)
{
	for (int i = 0; i < szSound; ++i)
	{
		WCHAR buffer[255];
		swprintf_s(buffer, L"setaudio %d_%d volume to %d", soundIndex, i, volume);
		mciSendString(buffer, 0, 0, 0);
	}
}
