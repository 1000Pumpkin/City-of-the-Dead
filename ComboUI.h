#pragma once
class ComboUI : public Object
{
public:
	Sprite baseSprite;
	Sprite::RenderProperty rp;
	D3DXVECTOR2 destScale = D3DXVECTOR2(1.0f, 1.0f);
	Number number;
	int prevComboCount = 0;
	float comboTime = 1.0f;
	int szNum = 1;

public:
	ComboUI();

	virtual void Update(float deltaTime);
	virtual void Render();
};

