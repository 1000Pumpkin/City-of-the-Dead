#include "Core.h"

SkillEnemy1::SkillEnemy1(Character* caster) : Skill(caster)
{
	WCHAR buffer[255];
	swprintf_s(buffer, L"Assets/Character/%s/Body/Attack1/", caster->name.c_str());
	sprite.LoadAll(buffer);
	sprite.isLoop = false;

	maxLevel = 0;
	OnLevelChanged(0);
}

void SkillEnemy1::OnLevelChanged(int level)
{
	castingTime[0] = 1.0f;
	coolTime[0] = 0.5f;
}

void SkillEnemy1::OnCast(float ratio)
{
	sprite.AniInit();
}

void SkillEnemy1::OnCharge()
{
}

void SkillEnemy1::OnExecute()
{
}

void SkillEnemy1::OnCancel()
{
}

void SkillEnemy1::OnUpdate(float fixedDeltaTime)
{
	if (IsOnceTime(State::kCast, 0.1f, fixedDeltaTime))
	{
		std::shared_ptr<SkillObject> so(new SkillObject());
		Collider2D::AABB aabb;
		aabb.min = D3DXVECTOR2(-20.0f, 0.0f);
		aabb.max = D3DXVECTOR2(20.0f, 50.0f);
		Collider2D coli2D = Collider2D(so.get(), &aabb, nullptr, nullptr);
		so->bodies.push_back(coli2D);
		so->position = caster->position + D3DXVECTOR2(10.0f * caster->rp.scale.x, 0.0f);
		so->duration = 0.1f;
		so->elasticForce = D3DXVECTOR2(5.0f * caster->rp.scale.x, 0.0f);
		so->hitDuration = 0.25f;
		so->damage = 5;
		so->tag = caster->tag;
		so->isPenetrate = true;
		SceneManager::GetInstance().curScene->obm.AddObject(so);
	}
}

void SkillEnemy1::SpriteUpdate(float deltaTime)
{
	sprite.Update(deltaTime);
}

void SkillEnemy1::SpriteRender(const Sprite::RenderProperty& rp)
{
	sprite.Render(rp);
}
