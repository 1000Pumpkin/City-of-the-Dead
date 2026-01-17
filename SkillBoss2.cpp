#include "Core.h"

SkillBoss2::SkillBoss2(Character* caster, int spawnAddon) : Skill(caster)
{
	WCHAR buffer[255];
	swprintf_s(buffer, L"Assets/Character/%s/Body/BossAttack1/", caster->name.c_str());
	addonSprite.LoadAll(buffer);

	this->spawnAddon = spawnAddon;
	OnLevelChanged(0);
}

void SkillBoss2::OnLevelChanged(int level)
{
	if (level == 0)
		coolTime[0] = 20.0f;
}

void SkillBoss2::OnCast(float ratio)
{
	for (int i = 0; i < spawnAddon; ++i)
	{
		float degree = (spawnMaxRotate - spawnMinRotate) / ((float)spawnAddon - 1) * i + spawnMinRotate;
		D3DXVECTOR2 randomOffset(0.0f, 150.0f);
		D3DXVECTOR2 spawnPos = caster->position + D3DXVec2Rotation(randomOffset, degree);

		auto player = std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->player;
		auto dp = player->position - spawnPos;
		if (D3DXVec2Length(&dp) == 0.0f)
			dp = D3DXVECTOR2(1.0f, 0.0f);
		D3DXVec2Normalize(&dp, &dp);

		std::shared_ptr<SkillObject> so(new SkillObject());
		so->position = spawnPos;
		so->duration = 7.0f;
		so->damage = 10;
		so->soSprite = addonSprite;
		Collider2D::AABB aabb;
		aabb.min = D3DXVECTOR2(-10.0f, -10.0f);
		aabb.max = D3DXVECTOR2(10.0f, 10.0f);
		Collider2D coli2D = Collider2D(so.get(), &aabb, nullptr, nullptr);
		so->bodies.push_back(coli2D);
		so->hitDuration = 0.0f;
		so->tag = caster->tag;
		so->isPenetrate = false;
		so->target = player;
		so->moveSpeed = 50.0f;
		SceneManager::GetInstance().curScene->obm.AddObject(so);

		std::shared_ptr<Effect> eff(new Effect(L"Assets/CommonFX/ChargeFX/"));
		eff->position = spawnPos;
		SceneManager::GetInstance().curScene->obm.AddObject(eff);
	}
}

void SkillBoss2::OnCharge()
{
}

void SkillBoss2::OnExecute()
{
}

void SkillBoss2::OnCancel()
{
}

void SkillBoss2::OnUpdate(float fixedDeltaTime)
{
}

void SkillBoss2::SpriteUpdate(float deltaTime)
{
}

void SkillBoss2::SpriteRender(const Sprite::RenderProperty& rp)
{
}
