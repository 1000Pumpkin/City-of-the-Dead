#include "Core.h"

Effect::Effect(std::wstring path, bool up)
{
	effSprite.LoadAll(path);
	if (!up)
		type = Type::kAnimation;
	else
		type = Type::kUp;
}

Effect::Effect(const Sprite& spr)
{
	effSprite.LoadAll(spr.filePath);
	effSprite.curScene = spr.curScene;
	type = Type::kCapture;
}

Effect::Effect(std::wstring path, int number, D3DXVECTOR2 prevScale, D3DXVECTOR2 targetScale)
{
	int szNum = 1;
	int num = number;
	while (number / 10 > 0)
	{
		number /= 10;
		++szNum;
	}

	numData.LoadAll(path, szNum);
	numData.rp.scale = prevScale;
	numData.SetScale(targetScale);
	numData.SetNumber(num);
	type = Type::kNumber;
}

void Effect::Update(float deltaTime)
{
	if (type == Type::kAnimation)
	{
		if (effSprite.Update(deltaTime))
			isRemove = true;
	}
	else if (type == Type::kCapture)
	{
		rp.a -= 1.0f / duration * deltaTime;
		if (rp.a <= 0.0f)
		{
			rp.a = 0.0f;
			isRemove = true;
		}
	}
	else if (type == Type::kNumber)
	{
		duration -= deltaTime;
		if (duration <= 0.0f)
			isRemove = true;
		position.y += 50.0f * deltaTime;
		numData.Update(deltaTime);
	}
	else if (type == Type::kUp)
	{
		rp.a -= 1.0f / duration * deltaTime;
		if (rp.a <= 0.0f)
		{
			rp.a = 0.0f;
			isRemove = true;
		}
		position.y += 50.0f * deltaTime;
	}
}

void Effect::Render()
{
	if (type != Type::kNumber)
	{
		rp.pos = position;
		if (object)
			rp.pos += object->position;

		effSprite.Render(rp);
	}
	else
	{
		numData.position = position;
		if (object)
			numData.position += object->position;
		numData.Render();
	}
}
