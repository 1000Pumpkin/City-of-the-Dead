#pragma once
class Character : public Object
{
public:
	enum State
	{
		kStay,
		kMove,
		kJump,
		kHit,
		kMoveStop,
		kDead,
		kJumpDropped,
		kSkill
	};

	static constexpr WCHAR* stateStrs[State::kSkill] = {
		L"Stay",
		L"Move",
		L"Jump",
		L"Hit",
		L"MoveStop",
		L"Dead",
		L"JumpDropped"
	};

	std::wstring name;
	Sprite bodySprite[State::kSkill];
	Sprite armSprite[State::kSkill];
	State bodyState = State::kStay, bodyNextState = State::kStay;
	State armState = State::kStay, armNextState = State::kStay;
	Sprite::RenderProperty rp;

	Sprite hpSprite;
	D3DXVECTOR2 hpScale = D3DXVECTOR2(1.0f, 1.0f);
	D3DXVECTOR2 hpOffset = D3DXVECTOR2(0.0f, -20.0f);

public:
	float moveDir = 0.0f;
	float moveSpeed = 300.0f;
	float moveStopSlip = moveSpeed / 4.0f;
	float moveStopAniTime = 0.0f;
	float moveStopAniMaxTime = 0.3f;
	int curJumpCount = 0, nextJumpCount = 0;
	float forceGravity = 9.8f;
	int hp = 100, maxHp = 100;
	int level = 0, sp = 0;
	int exp = 0, maxExp = 100;
	float hitDuration = 0.0f;
	float hitDurationScale = 1.0f;
	float damageRate = 1.0f;
	bool isColliderGroundTile = true;
	bool isGravity = true;

	D3DXVECTOR2 elasticForce = D3DXVECTOR2(0.0f, 0.0f);
	float elasticScale = 1.0f;
	D3DXVECTOR2 velocity = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 linear = D3DXVECTOR2(0.0f, 0.0f);

public:
	Skill* curSkill = NULL;
	std::vector<Skill*> skills;
	static std::shared_ptr<Sound> hitSound;
	static std::shared_ptr<Sound> hit2Sound;

	bool CmdCast(Skill* skill);
	void OnHit(int damage, D3DXVECTOR2 elasticForce, float duration, std::wstring skillName = L"");

public:
	Character(std::wstring name);
	virtual ~Character();
	virtual void Update(float deltaTime);
	virtual void LateUpdate(float deltaTime);
	virtual void FixedUpdate(float fixedDeltaTime);
	virtual void Render();
	virtual void OnCollision(Collider2D& collision);
};

