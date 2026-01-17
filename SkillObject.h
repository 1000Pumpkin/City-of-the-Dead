#pragma once
class SkillObject : public Object
{
private:
	std::vector<Object*> hitters;

public:
	D3DXVECTOR2 moveDir = D3DXVECTOR2(0.0f, 0.0f);
	float moveSpeed = 0.0f;
	std::shared_ptr<Object> target = NULL;
	float duration = 0.0f;
	bool isPenetrate = false;
	int damage = 0;
	float hitDuration = 0.25f;
	D3DXVECTOR2 elasticForce = D3DXVECTOR2(0.0f, 0.0f);
	Sprite::RenderProperty rp;
	Sprite soSprite;
	Sprite hitEffSprite;
	std::wstring skillName = L"";

public:
	SkillObject() {};

	virtual void Update(float deltaTime);
	virtual void LateUpdate(float deltaTime);
	virtual void FixedUpdate(float fixedDeltaTime);
	virtual void Render();
	virtual void OnCollision(Collider2D& collision);
};

