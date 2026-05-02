#include "Core.h"

SkillBoss4::SkillBoss4(Character* caster) : Skill(caster)
{
	WCHAR buffer[255];
	swprintf_s(buffer, L"Assets/Character/%s/Body/Stay/", caster->name.c_str());
	sprite.LoadAll(buffer);

	effSprite.LoadAll(L"Assets/Character/EnemyBoss2/Body/BossAttack4/Aura/");

	isCancel = false;
	OnLevelChanged(0);
}

void SkillBoss4::OnLevelChanged(int level)
{
	if (level == 0)
		castingTime[0] = 5.0f;
}

void SkillBoss4::OnCast(float ratio)
{
}

void SkillBoss4::OnCharge()
{
}

void SkillBoss4::OnExecute()
{
}

void SkillBoss4::OnCancel()
{
}

void SkillBoss4::OnUpdate(float fixedDeltaTime)
{
	for (int i = 0; i < 5; ++i)
	{
		if (IsOnceTime(State::kCast, 0.2f + 0.1f * (float)i, fixedDeltaTime))
		{
			std::shared_ptr<Effect> eff(new Effect(sprite));
			eff->position = caster->position;
			eff->rp = caster->rp;
			eff->rp.a = 0.8f;
			eff->rp.r = 0.1f;
			eff->rp.g = 0.1f;
			eff->rp.scale = D3DXVECTOR2(1.2f, 1.2f);
			eff->duration = 1.0f;
			SceneManager::GetInstance().curScene->obm.AddObject(eff);
		}
	}

	if (curState == State::kCast && time >= 0.8f)
	{
		auto player = std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->player;
		auto dp = player->position - caster->position;
		auto dl = D3DXVec2Length(&dp);
		D3DXVec2Normalize(&dp, &dp);
		if (dl > 50.0f)
			caster->elasticForce = dp * 5.0f;

		if (IsOnceTime(State::kCast, 1.0f, fixedDeltaTime) ||
			IsOnceTime(State::kCast, 1.5f, fixedDeltaTime) ||
			IsOnceTime(State::kCast, 2.0f, fixedDeltaTime) ||
			IsOnceTime(State::kCast, 2.5f, fixedDeltaTime) ||
			IsOnceTime(State::kCast, 3.0f, fixedDeltaTime) || 
			IsOnceTime(State::kCast, 3.5f, fixedDeltaTime) || 
			IsOnceTime(State::kCast, 4.0f, fixedDeltaTime) || 
			IsOnceTime(State::kCast, 4.5f, fixedDeltaTime) ||
			IsOnceTime(State::kCast, 5.0f, fixedDeltaTime))
		{
			std::shared_ptr<Effect> bodyEff(new Effect(sprite));
			bodyEff->duration = 0.2f;
			bodyEff->position = caster->position;
			bodyEff->rp = caster->rp;
			bodyEff->rp.r *= 0.5f;
			bodyEff->rp.g *= 0.5f;
			SceneManager::GetInstance().curScene->obm.AddObject(bodyEff);

			std::shared_ptr<SkillObject> so(new SkillObject());
			Collider2D::AABB aabb;
			aabb.min = D3DXVECTOR2(-15.0f, 0.0f);
			aabb.max = D3DXVECTOR2(15.0f, 100.0f);
			Collider2D coli2D = Collider2D(so.get(), &aabb, nullptr, nullptr);
			so->bodies.push_back(coli2D);
			so->position = caster->position + dp * 50.0f;
			so->duration = 0.5f;
			so->elasticForce = dp * 5.0f;
			so->hitDuration = 0.5f;
			so->damage = 5;
			so->tag = caster->tag;
			so->isPenetrate = true;
			SceneManager::GetInstance().curScene->obm.AddObject(so);
		}
	}
}

void SkillBoss4::SpriteUpdate(float deltaTime)
{
	sprite.Update(deltaTime);
	effSprite.Update(deltaTime);
}

void SkillBoss4::SpriteRender(const Sprite::RenderProperty& rp)
{
	sprite.Render(rp);
	effSprite.Render(rp);
}
