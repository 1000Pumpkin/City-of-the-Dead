#include "Core.h"

Score::Score()
{
	number.LoadAll(L"Assets/UI/Number/", 8);
	number.SetScale(D3DXVECTOR2(0.8f, 0.8f));
	scoreSprite.LoadAll(L"Assets/UI/Game/Score.png");
}

void Score::AddScore(int num)
{
	score += num;
	number.SetNumber(score);
	number.rp.scale = D3DXVECTOR2(1.0f, 1.0f);
}

void Score::Update(float deltaTime)
{
	number.Update(deltaTime);
}

void Score::Render()
{
	number.position = D3DXVECTOR2(262.0f, 160.0f);
	number.Render();

	Sprite::RenderProperty rp;
	rp.pos = number.rp.pos - D3DXVECTOR2(50.0f, 0.0f);
	scoreSprite.Render(rp);
}
