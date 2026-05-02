#pragma once
class Enemy1 : public Character
{
public:
	float appearanceTime = 0.0f;

public:
	Enemy1();

	virtual void FixedUpdate(float fixedDeltaTime);
};

