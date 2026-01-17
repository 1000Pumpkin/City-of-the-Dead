#include "Core.h"

SkillBoss3::SkillBoss3(Character* caster, int shotBullet) : Skill(caster)
{
	WCHAR buffer[255];
	swprintf_s(buffer, L"Assets/Character/%s/Body/Stay/", caster->name.c_str());
	sprite.LoadAll(buffer);
	this->shotBullet = shotBullet;
	isCancel = false;
	OnLevelChanged(0);
}

void SkillBoss3::OnLevelChanged(int level)
{
	if (level == 0)
	{
		castingTime[0] = (float)shotBullet * 0.08f + 1.0f;
	}
}

void SkillBoss3::OnCast(float ratio)
{
	sprite.AniInit();
}

void SkillBoss3::OnCharge()
{
}

void SkillBoss3::OnExecute()
{
}

void SkillBoss3::OnCancel()
{
}

void SkillBoss3::OnUpdate(float fixedDeltaTime)
{
	for (int i = 0; i < 5; ++i)
	{
		if (IsOnceTime(State::kCast, 0.2f + 0.1f * (float)i, fixedDeltaTime))
		{
			std::shared_ptr<Effect> eff(new Effect(sprite));
			eff->position = caster->position;
			eff->rp = caster->rp;
			eff->rp.a = 0.8f;
			eff->rp.g = 0.1f;
			eff->rp.b = 0.1f;
			eff->rp.scale = D3DXVECTOR2(1.2f, 1.2f);
			eff->duration = 1.0f;
			SceneManager::GetInstance().curScene->obm.AddObject(eff);
		}
	}

	for (int i = 0; i < shotBullet; ++i)
	{
		if (IsOnceTime(State::kCast, 1.0f + 0.08f * (float)i, fixedDeltaTime))
		{
			auto player = std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->player;
			auto dp = player->position - caster->position;
			float degree = 360.0f / shotBullet * (float)i;

			for (int way = 0; way < 2; ++way)
			{
				std::shared_ptr<SkillObject> so(new SkillObject());
				auto offset = D3DXVec2Rotation(D3DXVECTOR2(0.0f, 40.0f), degree + way * 180.0f);
				so->position = caster->position + offset + D3DXVECTOR2(0.0f, 40.0f);
				so->duration = 15.0f;
				so->damage = 3;
				so->soSprite.LoadAll(L"Assets/Character/EnemyBoss1/Body/BossAttack2/Bullet/");
				Collider2D::AABB aabb;
				aabb.min = D3DXVECTOR2(-10.0f, -10.0f);
				aabb.max = D3DXVECTOR2(10.0f, 10.0f);
				Collider2D coli2D = Collider2D(so.get(), &aabb, nullptr, nullptr);
				so->bodies.push_back(coli2D);
				D3DXVec2Normalize(&offset, &offset);
				so->moveDir = offset;
				so->moveSpeed = 250.0f;
				so->elasticForce = offset * 3.0f;
				so->hitDuration = 0.5f;
				so->tag = caster->tag;
				so->isPenetrate = false;
				SceneManager::GetInstance().curScene->obm.AddObject(so);
			}
		}
	}
}

void SkillBoss3::SpriteUpdate(float deltaTime)
{
	sprite.Update(deltaTime);
}

void SkillBoss3::SpriteRender(const Sprite::RenderProperty& rp)
{
	sprite.Render(rp);
}
