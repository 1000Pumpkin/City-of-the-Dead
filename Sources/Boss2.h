#pragma once
class Boss2 : public Character
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
	Boss2();
	virtual void FixedUpdate(float fixedDeltaTime);
};

