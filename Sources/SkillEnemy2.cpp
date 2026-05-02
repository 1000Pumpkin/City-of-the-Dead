#include "Core.h"

SkillEnemy2::SkillEnemy2(Character* caster, std::wstring path) : Skill(caster)
{
	WCHAR buffer[255];
	swprintf_s(buffer, L"Assets/Character/%s/Body/Attack2/", caster->name.c_str());
	sprite.LoadAll(buffer);
	sprite.isLoop = false;
	bulletPath = path;

	maxLevel = 0;
	OnLevelChanged(0);
}

void SkillEnemy2::OnLevelChanged(int level)
{
	castingTime[0] = 0.8f;
	coolTime[0] = 3.0f;
}

void SkillEnemy2::OnCast(float ratio)
{
	sprite.AniInit();
}

void SkillEnemy2::OnCharge()
{
}

void SkillEnemy2::OnExecute()
{
}

void SkillEnemy2::OnCancel()
{
}

void SkillEnemy2::OnUpdate(float fixedDeltaTime)
{
	if (IsOnceTime(State::kCast, 0.3f, fixedDeltaTime))
	{
		std::shared_ptr<SkillObject> so(new SkillObject());
		Collider2D::AABB aabb;
		aabb.min = D3DXVECTOR2(-10.0f, 0.0f);
		aabb.max = D3DXVECTOR2(10.0f, 10.0f);
		Collider2D coli2D = Collider2D(so.get(), &aabb, nullptr, nullptr);
		so->soSprite.LoadAll(bulletPath);
		so->bodies.push_back(coli2D);
		so->position = caster->position + D3DXVECTOR2(10.0f * caster->rp.scale.x, 50.0f);
		so->duration = 2.0f;
		so->elasticForce = D3DXVECTOR2(5.0f * caster->rp.scale.x, 0.0f);
		so->hitDuration = 0.25f;
		so->damage = 5;
		so->moveDir = D3DXVECTOR2(caster->rp.scale.x, 0.0f);
		so->moveSpeed = 600.0f;
		so->rp.scale.x = caster->rp.scale.x;
		so->tag = caster->tag;
		so->isPenetrate = true;
		SceneManager::GetInstance().curScene->obm.AddObject(so);
	}
}

void SkillEnemy2::SpriteUpdate(float deltaTime)
{
	sprite.Update(deltaTime);
}

void SkillEnemy2::SpriteRender(const Sprite::RenderProperty& rp)
{
	sprite.Render(rp);
}
