#pragma once
class Number : public Object
{
private:
	int numWidth = 0;
	int numHeight = 0;

public:
	std::vector<Sprite> nums;
	Sprite::RenderProperty rp;
	D3DXVECTOR2 destScale;
	int num = 0;
	int szNum = 0;

public:
	void LoadAll(std::wstring path, int szNum);
	void SetScale(D3DXVECTOR2 scale);
	void SetNumber(int number);
	virtual void Update(float deltaTime);
	virtual void Render();
};

