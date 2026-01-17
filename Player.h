#pragma once
class Player : public Character
{
public:
	float moveEffTime = 0.0f;
	int comboCount = 0;

public:
	Player();

	virtual void Update(float deltaTime);
	virtual void LateUpdate(float deltaTime);
	virtual void FixedUpdate(float fixedDeltaTime);
	virtual void OnCollision(Collider2D& collision);
};

