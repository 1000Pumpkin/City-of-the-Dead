#include "Core.h"

SkillEnemy3::SkillEnemy3(Character* caster) : Skill(caster)
{
	WCHAR buffer[255];
	swprintf_s(buffer, L"Assets/Character/%s/Body/Attack3/", caster->name.c_str());
	sprite.LoadAll(buffer);
	sprite.isLoop = false;
	effSprite.LoadAll(L"Assets/CommonFX/ChargeFX/");

	maxLevel = 0;
	isCancel = false;
	OnLevelChanged(0);
}

void SkillEnemy3::OnLevelChanged(int level)
{
	chargeTime[0] = 1.0f;
	castingTime[0] = 0.8f;
	coolTime[0] = 6.0f;
}

void SkillEnemy3::OnCast(float ratio)
{
	sprite.curScene = 1;

	auto player = std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->player;
	auto dp = player->position - caster->position;
	D3DXVec2Normalize(&dp, &dp);
	caster->elasticForce += dp * 30.0f;
	castPos = caster->position;
}

void SkillEnemy3::OnCharge()
{
	sprite.AniInit();
}

void SkillEnemy3::OnExecute()
{
}

void SkillEnemy3::OnCancel()
{
}

void SkillEnemy3::OnUpdate(float fixedDeltaTime)
{
	if (IsOnceTime(State::kCast, 0.1f, fixedDeltaTime))
	{
		auto player = std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->player;
		D3DXVECTOR2 dp = player->position - caster->position;
		if (dp != D3DXVECTOR2(0.0f, 0.0f))
		{
			std::shared_ptr<Effect> eff(new Effect(L"Assets/CommonFX/SlashAttack1FX/"));
			eff->position = caster->position + D3DXVECTOR2(0.0f, 20.0f);
			eff->rp = caster->rp;
			eff->rp.scale.x = caster->rp.scale.x * 1.3f;
			D3DXVec2Normalize(&dp, &dp);
			float degree = D3DXVec2Angle(dp);
			if (eff->rp.scale.x < 0)
				degree = -degree + 180;
			eff->rp.rotate = degree - 90.f;
			SceneManager::GetInstance().curScene->obm.AddObject(eff);
		}
	}

	if (IsOnceTime(State::kCast, 0.2f, fixedDeltaTime))
	{
		std::shared_ptr<SkillObject> so(new SkillObject());
		auto dp = caster->position - castPos;
		for (int i = 0; i < 5; ++i)
		{
			Collider2D::AABB aabb;
			aabb.min = D3DXVECTOR2(-40.0f, 0.0f) + dp / 5.0f * (float)i;
			aabb.max = D3DXVECTOR2(40.0f, 80.0f) + dp / 5.0f * (float)i;
			Collider2D coli2D = Collider2D(so.get(), &aabb, nullptr, nullptr);
			so->bodies.push_back(coli2D);
		}
		so->position = castPos;
		so->duration = 0.1f;
		D3DXVec2Normalize(&dp, &dp);
		so->elasticForce = dp * 10.0f;
		so->hitDuration = 0.25f;
		so->damage = 15;
		so->tag = caster->tag;
		so->isPenetrate = true;
		SceneManager::GetInstance().curScene->obm.AddObject(so);
	}
}

void SkillEnemy3::SpriteUpdate(float deltaTime)
{
	effSprite.Update(deltaTime);
}

void SkillEnemy3::SpriteRender(const Sprite::RenderProperty& rp)
{
	sprite.Render(rp);
	if (curState == State::kCharge)
	{
		Sprite::RenderProperty _rp = rp;
		_rp.pos.x -= 30.0f;
		effSprite.Render(_rp);
	}
}
