#pragma once
class Score : public Object
{
public:
	int score = 0;
	Number number;
	Sprite scoreSprite;

public:
	Score();
	void AddScore(int num);
	virtual void Update(float deltaTime);
	virtual void Render();
};

