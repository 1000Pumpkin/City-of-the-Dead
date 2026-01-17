#include "Core.h"

SkillBoss1::SkillBoss1(Character* caster, int spawnBullet) : Skill(caster)
{
	WCHAR buffer[255];
	swprintf_s(buffer, L"Assets/Character/%s/Body/Stay/", caster->name.c_str());
	sprite.LoadAll(buffer);
	bulletSprite.LoadAll(L"Assets/Character/EnemyBoss1/Body/BossAttack3/Bullet");
	this->spawnBullet = spawnBullet;
	isCancel = false;
	OnLevelChanged(0);
}

void SkillBoss1::OnLevelChanged(int level)
{
	if (level == 0)
	{
		castingTime[0] = 2.0f;
	}
}

void SkillBoss1::OnCast(float ratio)
{
	bullets.clear();
}

void SkillBoss1::OnCharge()
{
}

void SkillBoss1::OnExecute()
{
}

void SkillBoss1::OnCancel()
{
	for (auto bullet : bullets)
	{
		if (bullet != NULL)
		{
			bullet->isRemove = true;
		}
	}
	bullets.clear();
}

void SkillBoss1::OnUpdate(float fixedDeltaTime)
{
	for (int i = 0; i < spawnBullet; ++i)
	{
		if (IsOnceTime(State::kCast, 0.1f + 0.1f * (float)i, fixedDeltaTime))
		{
			float degree = (spawnMaxRotate - spawnMinRotate) / ((float)spawnBullet - 1) * i + spawnMinRotate;
			D3DXVECTOR2 randomOffset(0.0f, 150.0f);
			D3DXVECTOR2 spawnPos = caster->position + D3DXVec2Rotation(randomOffset, degree);

			auto player = std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->player;
			auto dp = player->position - spawnPos;
			if (D3DXVec2Length(&dp) == 0.0f)
				dp = D3DXVECTOR2(1.0f, 0.0f);
			D3DXVec2Normalize(&dp, &dp);

			std::shared_ptr<SkillObject> so(new SkillObject());
			so->position = spawnPos;
			so->duration = 20.0f;
			so->damage = 10;
			so->soSprite = bulletSprite;
			float rot = D3DXVec2Angle(dp);
			if (dp.x < 0) rot *= -1.f;
			so->rp.rotate = rot - 180.f;
			Collider2D::AABB aabb;
			aabb.min = D3DXVECTOR2(-10.0f, 0.0f);
			aabb.max = D3DXVECTOR2(10.0f, 10.0f);
			Collider2D coli2D = Collider2D(so.get(), &aabb, nullptr, nullptr);
			so->bodies.push_back(coli2D);
			so->moveDir = dp;
			so->isActiveCollider = false;
			so->elasticForce = dp * 7.0f;
			so->hitDuration = 0.5f;
			so->tag = caster->tag;
			so->isPenetrate = false;
			SceneManager::GetInstance().curScene->obm.AddObject(so);
			bullets.push_back(so);

			std::shared_ptr<Effect> eff(new Effect(L"Assets/CommonFX/ChargeFX/"));
			eff->position = spawnPos;
			SceneManager::GetInstance().curScene->obm.AddObject(eff);
		}

		if (IsOnceTime(State::kCast, (2.0f - 0.1f * spawnBullet) + 0.1f * (float)i, fixedDeltaTime))
		{
			bullets[i]->moveSpeed = 500.0f;
			bullets[i]->isActiveCollider = true;
			bullets[i] = NULL;
		}
	}
}

void SkillBoss1::SpriteUpdate(float deltaTime)
{
	sprite.Update(deltaTime);
}

void SkillBoss1::SpriteRender(const Sprite::RenderProperty& rp)
{
	sprite.Render(rp);
}
