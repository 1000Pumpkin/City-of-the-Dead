#include "Core.h"

Player::Player() : Character(L"Player")
{
	tag = L"Player";

	skills.push_back(new SkillAttack1(this));
	skills.push_back(new SkillAttack2(this));
	skills.push_back(new SkillDash(this));

	Collider2D::AABB aabb;
	aabb.min = D3DXVECTOR2(-10.0f, 0.0f);
	aabb.max = D3DXVECTOR2(10.0f, 65.0f);
	bodies.push_back(Collider2D(this, &aabb, nullptr, nullptr));
}

void Player::Update(float deltaTime)
{
	moveDir = 0.0f;
	if (KeyManager::GetInstance().GetKeyState('A'))
	{
		moveDir -= 1.0f;
	}
	if (KeyManager::GetInstance().GetKeyState('D'))
	{
		moveDir += 1.0f;
	}
	if (KeyManager::GetInstance().GetKeyState(VK_SPACE) == KeyState::Down)
	{
		++nextJumpCount;
	}
	if (KeyManager::GetInstance().GetKeyState(VK_LBUTTON) == KeyState::Down)
	{
		CmdCast(skills[0]);
	}
	if (KeyManager::GetInstance().GetKeyState(VK_RBUTTON) == KeyState::Down)
	{
		CmdCast(skills[1]);
	}
	if (KeyManager::GetInstance().GetKeyState(VK_RBUTTON) == KeyState::Up)
	{
		if (curSkill == skills[1])
			CmdCast(skills[1]);
	}
	if (KeyManager::GetInstance().GetKeyState(VK_SHIFT) == KeyState::Down)
	{
		if (!skills[2]->IsCasting())
		{
			if (curSkill && curSkill->Cancel())
			{
				bodyState = State::kStay;
				curSkill = NULL;
			}
			CmdCast(skills[2]);
		}
	}
	Character::Update(deltaTime);
}

void Player::LateUpdate(float deltaTime)
{
	Character::LateUpdate(deltaTime);
}

void Player::FixedUpdate(float fixedDeltaTime)
{
	Character::FixedUpdate(fixedDeltaTime);

	static const float moveEffInterval = 0.2f;
	if (bodyState == State::kMove)
	{
		moveEffTime -= fixedDeltaTime;
		if (moveEffTime <= 0.0f)
		{
			moveEffTime = moveEffInterval;
			std::shared_ptr<Effect> eff(new Effect(L"Assets/CommonFX/RunFX/"));
			eff->position = position + D3DXVECTOR2(0.0f, -10.0f);
			eff->rp = rp;
			SceneManager::GetInstance().curScene->obm.AddObject(eff);
		}
	}

	if (!Direct3D::GetInstance().isCamHandling)
	{
		Direct3D::GetInstance().camDestPos = position + D3DXVECTOR2(100.0f * rp.scale.x, 100.0f);
		Direct3D::GetInstance().camDestScale = D3DXVECTOR2(1.0f, 1.0f);
	}
}

void Player::OnCollision(Collider2D& collision)
{
	if (collision.object->tag == L"Hp")
	{
		std::shared_ptr<Effect> eff(new Effect(L"Assets/CommonFX/GetHpFX/", true));
		eff->position = D3DXVECTOR2(0.0f, 50.0f);
		eff->duration = 0.5f;
		eff->object = this;
		SceneManager::GetInstance().curScene->obm.AddObject(eff);

		hp += maxHp * 0.2f;
		if (hp > maxHp)
			hp = maxHp;

		collision.object->isRemove = true;
	}
	if (collision.object->tag == L"Exp")
	{
		std::shared_ptr<Effect> eff(new Effect(L"Assets/CommonFX/GetExpFX/", true));
		eff->position = D3DXVECTOR2(0.0f, 50.0f);
		eff->duration = 0.5f;
		eff->object = this;
		SceneManager::GetInstance().curScene->obm.AddObject(eff);

		exp += maxExp * 0.2f;
		if (exp > maxExp)
			exp = maxExp;

		collision.object->isRemove = true;
	}
}
