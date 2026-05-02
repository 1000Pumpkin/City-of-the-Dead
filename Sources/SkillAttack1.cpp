#include "Core.h"

SkillAttack1::SkillAttack1(Character* caster) : Skill(caster)
{
	WCHAR buffer[255];
	for (int i = 0; i < 3; ++i)
	{
		swprintf_s(buffer, L"Assets/Character/Player/Arms/Attack/%d/", i);
		armSprite[i].LoadAll(buffer);
		armSprite[i].isLoop = false;
		armSprite[i].aniMaxTime = 0.05f;
	}

	maxLevel = 2;
	casterMove = true;

	attackSound.reset(new Sound(L"Assets/SoundFX/Slash.mp3", 5));

	OnLevelChanged(0);
}

void SkillAttack1::OnLevelChanged(int level)
{
	if (level == 0)
	{
		maxPhase = 1;
		castingTime[0] = 0.2f;
		phaseTime[0] = 0.1f;
		castingTime[1] = 0.2f;
	}
	else if (level == 1 || level == 2)
	{
		maxPhase = 2;
		castingTime[0] = 0.2f;
		phaseTime[0] = 0.1f;
		castingTime[1] = 0.2f;
		phaseTime[1] = 0.1f;
		castingTime[2] = 0.2f;
	}
}

void SkillAttack1::OnCast(float ratio)
{
	attackSound->Play(false);
	armSprite[curPhase].AniInit();
	D3DXVECTOR2 t = KeyManager::GetInstance().worldCursorPos - caster->position;
	D3DXVECTOR2 n;
	D3DXVec2Normalize(&n, &t);
	caster->elasticForce += n * 5.0f;
}

void SkillAttack1::OnCharge()
{
}

void SkillAttack1::OnExecute()
{
}

void SkillAttack1::OnCancel()
{
	attackSound->Stop();
}

void SkillAttack1::OnUpdate(float fixedDeltaTime)
{
	if (curState == State::kCast)
	{
		if (IsOnceTime(State::kCast, 0.05f, fixedDeltaTime) ||
			IsOnceTime(State::kCast, 0.07f, fixedDeltaTime) ||
			IsOnceTime(State::kCast, 0.1f, fixedDeltaTime) ||
			IsOnceTime(State::kCast, 0.15f, fixedDeltaTime))
		{
			std::shared_ptr<Effect> bodyEff(new Effect(caster->bodySprite[caster->bodyState]));
			bodyEff->duration = 0.2f;
			bodyEff->position = caster->position;
			bodyEff->rp = caster->rp;
			bodyEff->rp.a *= 0.5f;
			bodyEff->rp.r *= 0.5f;
			bodyEff->rp.g *= 0.5f;
			SceneManager::GetInstance().curScene->obm.AddObject(bodyEff);

			std::shared_ptr<Effect> armEff(new Effect(armSprite[curPhase]));
			armEff->duration = 0.2f;
			armEff->position = caster->position;
			armEff->rp = caster->rp;
			armEff->rp.a *= 0.5f;
			armEff->rp.r *= 0.5f;
			armEff->rp.g *= 0.5f;
			SceneManager::GetInstance().curScene->obm.AddObject(armEff);
		}

		if (IsOnceTime(State::kCast, 0.1f, fixedDeltaTime))
		{
			std::shared_ptr<SkillObject> so(new SkillObject());
			Collider2D::AABB aabb;
			aabb.min = D3DXVECTOR2(-40.0f, 0.0f);
			aabb.max = D3DXVECTOR2(40.0f, 90.0f);
			if (level == 2)
			{
				aabb.min = D3DXVECTOR2(-40.0f, -50.0f);
				aabb.max = D3DXVECTOR2(40.0f, 80.0f);
			}
			Collider2D coli2D = Collider2D(so.get(), &aabb, nullptr, nullptr);
			so->bodies.push_back(coli2D);
			so->position = caster->position + D3DXVECTOR2(30.0f * caster->rp.scale.x, 0.0f);
			so->duration = 0.1f;
			so->elasticForce = D3DXVECTOR2(6.0f * caster->rp.scale.x, 0.0f);
			if (level > 0 && curPhase == 2)
				so->elasticForce.y += 10.0f;
			so->hitDuration = 0.8f;
			so->damage = int(10.0f * caster->damageRate);
			if (level == 2)
			{
				so->soSprite.LoadAll(L"Assets/Character/Player/Arms/Attack/SlashFX/");
				so->moveDir = D3DXVECTOR2(caster->rp.scale.x, 0.0f);
				so->moveSpeed = 350.0f;
				so->duration = 0.4f;
				so->rp.scale.x = caster->rp.scale.x;
				so->position.y += 30.0f;
			}
			so->tag = caster->tag;
			so->isPenetrate = true;
			so->skillName = L"Attack";
			SceneManager::GetInstance().curScene->obm.AddObject(so);
		}
	}
}

void SkillAttack1::SpriteUpdate(float deltaTime)
{
	armSprite[curPhase].Update(deltaTime);
}

void SkillAttack1::SpriteRender(const Sprite::RenderProperty& rp)
{
	armSprite[curPhase].Render(rp);
}
