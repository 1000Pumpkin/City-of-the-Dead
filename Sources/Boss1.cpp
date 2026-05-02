#include "Core.h"

Boss1::Boss1() : Character(L"EnemyBoss1")
{
	tag = L"Enemy";
	moveSpeed = 50.0f;
	moveStopSlip = 0.0f;
	isGravity = false;
	isColliderGroundTile = false;
	rp.a = 0.0f;

	Collider2D::AABB aabb;
	aabb.min = D3DXVECTOR2(-15.0f, 0.0f);
	aabb.max = D3DXVECTOR2(15.0f, 100.0f);
	bodies.push_back(Collider2D(this, &aabb, nullptr, nullptr));

	skills.push_back(new SkillBoss1(this, 8));
	skills.push_back(new SkillBoss2(this, 2));
	skills.push_back(new SkillBoss3(this, 20));

	hpScale = D3DXVECTOR2(2.0f, 1.0f);
	maxHp = hp = 400;
	hitDurationScale = 0.0f;
	elasticScale = 0.0f;
	isActiveCollider = false;

	std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->bgmSound->Stop();
	std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->bossBgmSound->Play(true);
}

void Boss1::FixedUpdate(float fixedDeltaTime)
{
	if (appearanceTime <= 5.0f)
	{
		spawnPos = position;
		appearanceTime += fixedDeltaTime;
		rp.a += fixedDeltaTime;
		if (rp.a >= 1.0f)
			rp.a = 1.0f;
		Direct3D::GetInstance().isCamHandling = true;
		Direct3D::GetInstance().camDestPos = position + D3DXVECTOR2(0.0f, 50.0f);
		Direct3D::GetInstance().camDestScale = D3DXVECTOR2(1.5f, 1.5f);
	}
	else if (bodyState == State::kDead)
	{
		deadTime += fixedDeltaTime;
		deadEffTime += fixedDeltaTime;
		position.y -= 30.0f * fixedDeltaTime;
		rp.a -= fixedDeltaTime * 0.2f;
		if (rp.a <= 0.0f)
			rp.a = 0.0f;
		if (deadTime <= 5.0f)
		{
			Direct3D::GetInstance().isCamHandling = true;
			Direct3D::GetInstance().camDestPos = position + D3DXVECTOR2(0.0f, 50.0f);
			Direct3D::GetInstance().camDestScale = D3DXVECTOR2(1.5f, 1.5f);
		}
		else
		{
			std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->clear = true;
			Direct3D::GetInstance().isCamHandling = false;
		}
		if (deadEffTime > 0.2f)
		{
			std::shared_ptr<Effect> eff(new Effect(L"Assets/CommonFX/SmokeExplodeFX/"));
			eff->position = position + D3DXVECTOR2((float)(rand() % 40) - 20.0f, (float)(rand() % 150) - 30.0f);
			eff->rp = rp;
			SceneManager::GetInstance().curScene->obm.AddObject(eff);
			deadEffTime = 0.0f;
		}
	}
	else
	{
		Direct3D::GetInstance().isCamHandling = false;
		isActiveCollider = true;
		CmdCast(skills[1]);
		if (patternTime >= 0.0f && patternTime - fixedDeltaTime < 0.0f)
		{
			D3DXVECTOR2 randomOffset(0.0f, 100.0f);
			float randomDegree[] = { 0.0f, 45.0f, 90.0f, 135.0f, 180.0f, 225.0f, 270.0f, 315.0f };
			movePos = spawnPos + D3DXVec2Rotation(randomOffset, randomDegree[rand() % 8]);
		}
		if (patternTime <= -2.0f)
		{
			CmdCast(rand() % 2 == 0 ? skills[0] : skills[2]);
			patternTime = 6.0f;
		}
		patternTime -= fixedDeltaTime;
		position += (movePos - position) * 0.5f * fixedDeltaTime;
	}

	floatationTime += fixedDeltaTime;
	floatation = D3DXVECTOR2(sinf(floatationTime), cosf(floatationTime)) * 10.0f * fixedDeltaTime;
	position += floatation;

	Character::FixedUpdate(fixedDeltaTime);
}
