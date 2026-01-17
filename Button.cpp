#include "Core.h"

void Button::Update(float deltaTime)
{
	if (!eventHandler)
	{
		btnSprite.curScene = State::Disable;
		return;
	}

	auto cursorPos = KeyManager::GetInstance().cursorPos;
	float width = (float)btnSprite.textures[state]->info.Width;
	float height = (float)btnSprite.textures[state]->info.Height;
	auto min = position + D3DXVECTOR2(width * -0.5f, height * -0.5f);
	auto max = position + D3DXVECTOR2(width * 0.5f, height * 0.5f);
	if (IsCollisionAABBPos(min, max, cursorPos))
	{
		if (state == State::None)
		{
			state = State::Hover;
			if (OnHover)
				OnHover();
		}
		if (KeyManager::GetInstance().GetKeyState(VK_LBUTTON) == KeyState::Down)
		{
			state = State::Click;
			if (OnClick)
				OnClick();
		}
	}
	else
	{
		state = State::None;
	}
	btnSprite.curScene = state;
}

void Button::Render()
{
	Sprite::RenderProperty rp;
	rp.pos = position;
	btnSprite.Render(rp);
}
