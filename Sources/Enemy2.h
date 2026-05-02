#pragma once
class Enemy2 : public Character
{
public:
	float appearanceTime = 0.0f;

public:
	Enemy2();

	virtual void FixedUpdate(float fixedDeltaTime);
};

