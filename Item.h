#pragma once
class Item : public Object
{
public:
	enum Type
	{
		Hp,
		Exp
	} type;

	Sprite sprite;
	Sprite::RenderProperty rp;
	float vSpeed = 100.0f;
	float defaultVSpeed = 100.0f;
	float aliveTime = 0.0f;

public:
	Item(int type, D3DXVECTOR2 pos);

	virtual void Update(float deltaTime);
	virtual void LateUpdate(float deltaTime);
	virtual void FixedUpdate(float fixedDeltaTime);
	virtual void Render();
	virtual void OnCollision(Collider2D& collision);
};

