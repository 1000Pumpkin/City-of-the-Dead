#include "Core.h"

SkillDash::SkillDash(Character* caster) : Skill(caster)
{
	bodySprite.LoadAll(L"Assets/Character/Player/Body/Dash/");
	bodySprite.isLoop = false;
	armSprite.LoadAll(L"Assets/Character/Player/Arms/Dash/");
	armSprite.isLoop = false;

	maxLevel = 1;
	OnLevelChanged(0);
}

void SkillDash::OnLevelChanged(int level)
{
	if (level == 0)
	{
		castingTime[0] = 0.2f;
		coolTime[0] = 1.0f;
	}
	else if (level == 1)
	{
		maxPhase = 1;
		castingTime[0] = 0.2f;
		coolTime[0] = 1.0f;
		phaseTime[0] = 0.1f;
		castingTime[1] = 0.2f;
		coolTime[1] = 1.0f;
	}
}

void SkillDash::OnCast(float ratio)
{
	bodySprite.AniInit();
	armSprite.AniInit();
	caster->isActiveCollider = false;
	caster->elasticForce.x += caster->rp.scale.x * 20.0f;
}

void SkillDash::OnCharge()
{
}

void SkillDash::OnExecute()
{
	caster->isActiveCollider = true;
}

void SkillDash::OnCancel()
{
	caster->isActiveCollider = true;
}

void SkillDash::OnUpdate(float fixedDeltaTime)
{
	if (curState == State::kCast)
	{
		if (IsOnceTime(State::kCast, 0.05f, fixedDeltaTime) ||
			IsOnceTime(State::kCast, 0.07f, fixedDeltaTime) ||
			IsOnceTime(State::kCast, 0.1f, fixedDeltaTime) ||
			IsOnceTime(State::kCast, 0.15f, fixedDeltaTime))
		{
			std::shared_ptr<Effect> bodyEff(new Effect(bodySprite));
			bodyEff->duration = 0.2f;
			bodyEff->position = caster->position;
			bodyEff->rp = caster->rp;
			bodyEff->rp.a *= 0.8f;
			bodyEff->rp.r *= 0.8f;
			bodyEff->rp.g *= 0.8f;
			SceneManager::GetInstance().curScene->obm.AddObject(bodyEff);

			std::shared_ptr<Effect> armEff(new Effect(armSprite));
			armEff->duration = 0.2f;
			armEff->position = caster->position;
			armEff->rp = caster->rp;
			armEff->rp.a *= 0.8f;
			armEff->rp.r *= 0.8f;
			armEff->rp.g *= 0.8f;
			SceneManager::GetInstance().curScene->obm.AddObject(armEff);
		}
	}
}

void SkillDash::SpriteUpdate(float deltaTime)
{
	bodySprite.Update(deltaTime);
	armSprite.Update(deltaTime);
}

void SkillDash::SpriteRender(const Sprite::RenderProperty& rp)
{
	bodySprite.Render(rp);
	armSprite.Render(rp);
}
