#pragma once
class RankScene : public Scene
{
public:
	class RankInfo
	{
	public:
		std::wstring names[3] = { L"AAA", L"AAA", L"AAA" };
		int scores[3] = { 0, 0, 0 };
	};

	int score;
	Sprite bgSprite;
	std::shared_ptr<Alphabet> alphabets[3];
	std::shared_ptr<Number> numbers[3];

	int overwriteScoreIDX = -1;

public:
	RankScene(int score = 0);
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render();
	virtual void UiRender();

	static void Save(const std::wstring& name, const int& score);
	static void Load(RankInfo& ri);
};

