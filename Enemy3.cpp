#include "Core.h"

Enemy3::Enemy3() : Character(L"Enemy3")
{
	tag = L"Enemy";

	moveSpeed = 200.0f;
	moveStopSlip = 0.0f;

	skills.push_back(new SkillEnemy1(this));
	skills.push_back(new SkillEnemy2(this, L"Assets/Character/Enemy3/Bullet/"));
	skills.push_back(new SkillEnemy3(this));

	Collider2D::AABB aabb;
	aabb.min = D3DXVECTOR2(-10.0f, 0.0f);
	aabb.max = D3DXVECTOR2(10.0f, 65.0f);
	bodies.push_back(Collider2D(this, &aabb, nullptr, nullptr));

	hp = maxHp = 60;
}

void Enemy3::FixedUpdate(float fixedDeltaTime)
{
	appearanceTime += fixedDeltaTime;

	auto player = std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->player;
	auto dp = player->position - position;
	auto dl = D3DXVec2Length(&dp);

	if (appearanceTime >= 1.0f)
	{
		moveDir = 0.0f;

		if (dl < 500.0f)
		{
			if (dl > 10.0f && dp.x > 0.0f)
				moveDir = 1.0f;
			else if (dl > 10.0f && dp.x < 0.0f)
				moveDir = -1.0f;
			if (dp.y >= TILEHEIGHT)
				nextJumpCount = 1;
			if (dl < 40.0f)
				CmdCast(skills[0]);
			if (dl > 150.0f && dl < 300.0f)
				if (!skills[2]->IsCasting())
					CmdCast(skills[2]);
			CmdCast(skills[1]);
		}
	}
	else
	{
		if (dl > 10.0f && dp.x > 0.0f)
			rp.scale.x = 1.0f;
		else if (dl > 10.0f && dp.x < 0.0f)
			rp.scale.x = -1.0f;
	}

	Character::FixedUpdate(fixedDeltaTime);
}
