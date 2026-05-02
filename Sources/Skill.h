#pragma once

class Character;

class Skill
{
protected:
	enum State
	{
		kReady,
		kCharge,
		kCast,
		kPhase,
		kCancel,
		kCool
	};

	Character* caster;
	float time = 0.0f;
	State curState = State::kReady;
	State nextState = State::kReady;

	int level = 0, maxLevel = 0;
	int curPhase = 0, maxPhase = 0;
	static const int limitPhase = 10;
	float castingTime[limitPhase] = {};
	float chargeTime[limitPhase] = {};
	float coolTime[limitPhase] = {};
	float phaseTime[limitPhase] = {};
	bool isCancel = true;

public:
	bool casterMove = false;

protected:
	virtual void OnLevelChanged(int level) = 0;
	virtual void OnCast(float ratio) = 0;
	virtual void OnCharge() = 0;
	virtual void OnExecute() = 0;
	virtual void OnCancel() = 0;
	virtual void OnUpdate(float fixedDeltaTime) = 0;

public:
	virtual ~Skill() {};
	virtual void SpriteUpdate(float deltaTime) = 0;
	virtual void SpriteRender(const Sprite::RenderProperty& rp) = 0;
	bool IsSkillDone();
	bool IsCasting();
	bool IsCoolTime();
	bool IsOnceTime(State state, float t, float fixedDeltaTime);

public:
	void LevelUp();
	bool Casting();
	bool Cancel(bool force = false);

	Skill(Character* caster);
	void FixedUpdate(float fixedDeltaTime);
};

