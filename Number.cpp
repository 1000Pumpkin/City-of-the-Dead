#include "Core.h"

void Number::LoadAll(std::wstring path, int szNum)
{
	Sprite numSprite;
	numSprite.LoadAll(path);

	nums.resize(szNum);
	for (int i = 0; i < szNum; ++i)
		nums[i] = numSprite;

	this->szNum = szNum;
	numWidth = numSprite.textures[0]->info.Width;
	numHeight = numSprite.textures[0]->info.Height;
}

void Number::SetScale(D3DXVECTOR2 scale)
{
	destScale = scale;
}

void Number::SetNumber(int number)
{
	num = number;
	for (int i = szNum - 1; i >= 0; --i)
	{
		nums[i].curScene = number % 10;
		number /= 10;
	}
}

void Number::Update(float deltaTime)
{
	rp.scale += (destScale - rp.scale) * 10.0f * deltaTime;
}

void Number::Render()
{
	float rWidth = numWidth * rp.scale.x * 0.8f;
	float posOffset = rWidth * 0.5f * (szNum - 1);
	for (int i = szNum - 1; i >= 0; --i)
	{
		rp.pos = position + D3DXVECTOR2(rWidth * i - posOffset, 0.0f);
		nums[i].Render(rp);
	}
}
