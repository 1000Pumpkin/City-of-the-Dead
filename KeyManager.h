#pragma once

enum KeyState
{
	None,
	Down,
	Press,
	Up
};

class KeyManager
{
private:
	KeyManager() {};
	KeyManager(const KeyManager&) = delete;
	void operator =(const KeyManager&) = delete;
	
public:
	static KeyManager& GetInstance()
	{
		static KeyManager instance;
		return instance;
	}

public:
	int keyState[256] = {};
	D3DXVECTOR2 cursorPos;
	D3DXVECTOR2 worldCursorPos;
	Sprite cursorSprite;
	Sprite::RenderProperty cursorProperty;

public:
	void Update(float deltaTime);
	int GetKeyState(int key);
	void UiRender();
};