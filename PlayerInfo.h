#pragma once
class PlayerInfo : public Object
{
public:
	Sprite hpSprite;
	Sprite expSprite;

public:
	PlayerInfo();

	virtual void Render();
};

