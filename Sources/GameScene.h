#pragma once

class PlayerPref
{
public:
	int level = 0;
	int score = 0;
	int exp = 0;
	int sp = 0;
	bool isLearnType[PlayerSpec::Type::Count] = {};
};

class GameScene : public Scene
{
public:
	enum StageState
	{
		Start,
		Fail,
		Clear,
		Count
	};

	int stage = 0;
	PlayerPref pref;
	std::shared_ptr<Player> player;
	std::shared_ptr<PlayerSpec> spec;
	std::shared_ptr<Score> score;

	StageState state = StageState::Start;
	Sprite stageSprite[StageState::Count];
	Sprite::RenderProperty stageProperty[StageState::Count];
	Sprite resultSprite;
	Sprite::RenderProperty resultProperty;
	Sprite tabSprite;
	Sprite::RenderProperty tabProprety;
	float resultTime = 0.0f;
	bool clear = false;
	bool isAniEnd = false;

	std::shared_ptr<Sound> bgmSound;
	std::shared_ptr<Sound> bossBgmSound;

public:
	GameScene(int stage, const PlayerPref& pref = PlayerPref());
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void FixedUpdate(float fixedDeltaTime);
	virtual void Render();
	virtual void UiRender();
};

