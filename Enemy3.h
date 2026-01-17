#pragma once
class Enemy3 : public Character
{
public:
	float appearanceTime = 0.0f;

public:
	Enemy3();

	virtual void FixedUpdate(float fixedDeltaTime);
};

