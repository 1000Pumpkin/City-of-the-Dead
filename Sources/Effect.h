#pragma once
class Effect : public Object
{
public:
	enum Type
	{
		kAnimation,
		kCapture,
		kNumber,
		kUp
	} type;

	Sprite effSprite;
	Sprite::RenderProperty rp;
	Number numData;
	float duration = 0.0f;
	Object* object = NULL;

	Effect(std::wstring path, bool up = false);
	Effect(const Sprite& spr);
	Effect(std::wstring path, int number, D3DXVECTOR2 prevScale, D3DXVECTOR2 targetScale);

	virtual void Update(float deltaTime);
	virtual void Render();
};

