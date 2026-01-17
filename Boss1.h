#pragma once
class Boss1 : public Character
{
public:
	float appearanceTime = 0.0f;
	float floatationTime = 0.0f;
	float deadTime = 0.0f;
	float deadEffTime = 0.0f;
	D3DXVECTOR2 floatation;
	D3DXVECTOR2 spawnPos;
	float patternTime = 0.0f;
	D3DXVECTOR2 movePos;

public:
	Boss1();
	virtual void FixedUpdate(float fixedDeltaTime);
};

