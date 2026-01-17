#include "Core.h"


void KeyManager::Update(float deltaTime)
{
	for (int i = 0; i < 256; ++i)
	{
		if (GetAsyncKeyState(i))
		{
			if (keyState[i] == KeyState::None)
				keyState[i] = KeyState::Down;
			else if (keyState[i] == KeyState::Down)
				keyState[i] = KeyState::Press;
		}
		else
		{
			if (keyState[i] == KeyState::Up)
				keyState[i] = KeyState::None;
			else if (keyState[i] > 0)
				keyState[i] = KeyState::Up;
		}
	}

	auto& D3D = Direct3D::GetInstance();
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(D3D.m_hWnd, &p);
	p.y = D3D.screenHeight - p.y;
	cursorPos = D3DXVECTOR2(
		p.x / (float)D3D.screenWidth * (float)D3D.resolutionWidth - (float)D3D.resolutionWidth * 0.5f,
		p.y / (float)D3D.screenHeight * (float)D3D.resolutionHeight - (float)D3D.resolutionHeight * 0.5f
	);
	D3DXVECTOR2 cp2(cursorPos.x / D3D.camScale.x, cursorPos.y / D3D.camScale.y);
	worldCursorPos = cp2 + D3D.camPos;

	cursorProperty.rotate += 360.0f * deltaTime;
}

int KeyManager::GetKeyState(int key)
{
	return keyState[key];
}

void KeyManager::UiRender()
{
	if (cursorSprite.maxScene == 0)
	{
		ShowCursor(false);
		cursorSprite.LoadAll(L"Assets/UI/Cursor/");
	}

	cursorProperty.pos = cursorPos;
	cursorSprite.Render(cursorProperty);
}
