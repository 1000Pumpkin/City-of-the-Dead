#pragma once
class MainScene : public Scene
{
public:
	enum BtnType
	{
		kStart,
		kHelp,
		kRank,
		kExit,
		kCount
	};

	std::wstring btnNames[BtnType::kCount] = {
		L"Start",
		L"Help",
		L"Rank",
		L"Exit"
	};

	std::shared_ptr<Button> btn[4];
	Sprite titleSprite;
	bool isShowHelp = false;
	Sprite helpSprite;

public:
	virtual void Init();
	virtual void Term();
	virtual void Update(float deltaTime);
	virtual void FixedUpdate(float fixedDeltaTime);
	virtual void Render();
	virtual void UiRender();
};

