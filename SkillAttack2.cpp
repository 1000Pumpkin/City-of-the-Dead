#include "Core.h"

SkillAttack2::SkillAttack2(Character* caster) : Skill(caster)
{
	bodySprite.LoadAll(L"Assets/Character/Player/Body/AttackP/");
	bodySprite.isLoop = false;
	armSprite.LoadAll(L"Assets/Character/Player/Arms/AttackP");
	armSprite.isLoop = false;

	chargeSprite.LoadAll(L"Assets/CommonFX/ChargeFX");

	maxLevel = 2;
	attackSound.reset(new Sound(L"Assets/SoundFX/Slash.mp3", 3));
	OnLevelChanged(0);
}

void SkillAttack2::OnLevelChanged(int level)
{
	if (level == 0)
	{
		castingTime[0] = 0.5f;
		chargeTime[0] = 2.0f;
		coolTime[0] = 1.0f;
	}
	else if (level == 1 || level == 2)
	{
		maxPhase = 1;
		castingTime[0] = 0.5f;
		chargeTime[0] = 2.0f;
		coolTime[0] = 1.0f;
		phaseTime[0] = 0.1f;

		castingTime[1] = 0.5f;
		chargeTime[1] = 2.0f;
		coolTime[1] = 2.0f;
	}
}

void SkillAttack2::OnCast(float ratio)
{
	Direct3D::GetInstance().isCamHandling = false;
	chargeEff = false;

	auto dp = KeyManager::GetInstance().worldCursorPos - caster->position;
	D3DXVec2Normalize(&dp, &dp);
	caster->elasticForce += dp * 50.0f;

	chargePos = caster->position;
	chargeRatio = ratio;

	attackSound->Play(false);
}

void SkillAttack2::OnCharge()
{
	Direct3D::GetInstance().isCamHandling = true;
	Direct3D::GetInstance().camDestScale = D3DXVECTOR2(1.1f, 1.1f);
}

void SkillAttack2::OnExecute()
{
}

void SkillAttack2::OnCancel()
{
	Direct3D::GetInstance().isCamHandling = false;
	Direct3D::GetInstance().camDestScale = D3DXVECTOR2(1.0f, 1.0f);
	attackSound->Stop();
}

void SkillAttack2::OnUpdate(float fixedDeltaTime)
{
	if (curState == State::kCharge)
	{
		auto dp = KeyManager::GetInstance().worldCursorPos - caster->position;
		if (D3DXVec2Length(&dp) > 100.0f)
		{
			D3DXVec2Normalize(&dp, &dp);
			Direct3D::GetInstance().camDestPos = caster->position + dp * 100.0f;
		}
		else
		{
			Direct3D::GetInstance().camDestPos = KeyManager::GetInstance().worldCursorPos;
		}

		if (IsOnceTime(State::kCharge, chargeTime[0] * 0.6f, fixedDeltaTime))
		{
			Direct3D::GetInstance().camDestScale = D3DXVECTOR2(1.2f, 1.2f);
			chargeEff = true;
		}
		if (time > chargeTime[0] * 0.6f)
		{
			Direct3D::GetInstance().camQuaken = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	if (curState == State::kCast)
	{
		if (IsOnceTime(State::kCast, 0.05f, fixedDeltaTime) ||
			IsOnceTime(State::kCast, 0.08f, fixedDeltaTime) ||
			IsOnceTime(State::kCast, 0.1f, fixedDeltaTime) ||
			IsOnceTime(State::kCast, 0.15f, fixedDeltaTime) ||
			IsOnceTime(State::kCast, 0.2f, fixedDeltaTime) ||
			IsOnceTime(State::kCast, 0.3f, fixedDeltaTime) ||
			IsOnceTime(State::kCast, 0.4f, fixedDeltaTime))
		{
			std::shared_ptr<Effect> bodyEff(new Effect(bodySprite));
			bodyEff->duration = 0.2f;
			bodyEff->position = caster->position;
			bodyEff->rp = caster->rp;
			bodyEff->rp.r *= 0.5f;
			bodyEff->rp.g *= 0.5f;
			SceneManager::GetInstance().curScene->obm.AddObject(bodyEff);

			std::shared_ptr<Effect> armEff(new Effect(armSprite));
			armEff->duration = 0.2f;
			armEff->position = caster->position;
			armEff->rp = caster->rp;
			armEff->rp.r *= 0.5f;
			armEff->rp.g *= 0.5f;
			SceneManager::GetInstance().curScene->obm.AddObject(armEff);
		}

		if (IsOnceTime(State::kCast, 0.1f, fixedDeltaTime))
		{
			D3DXVECTOR2 dp = D3DXVECTOR2(caster->position.x - chargePos.x, caster->position.y - chargePos.y);
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

		if (IsOnceTime(State::kCast, 0.1f, fixedDeltaTime))
		{
			std::shared_ptr<SkillObject> so(new SkillObject);
			auto dp = caster->position - chargePos;
			for (int i = 0; i < 5; ++i)
			{
				Collider2D::AABB aabb;
				aabb.min = D3DXVECTOR2(-40.0f, 0.0f) + dp / 5.0f * (float)i;
				aabb.max = D3DXVECTOR2(40.0f, 80.0f) + dp / 5.0f * (float)i;
				Collider2D coli2D = Collider2D(so.get(), &aabb, nullptr, nullptr);
				so->bodies.push_back(coli2D);
			}
			so->position = chargePos;
			so->duration = 0.1f;
			D3DXVec2Normalize(&dp, &dp);
			so->elasticForce = dp * (10.0f + (int)(10.0f * chargeRatio)) * 2.0f;
			so->hitDuration = 1.5f;
			so->damage = (int)((20.0f + 30.0f * chargeRatio) * caster->damageRate);
			so->tag = caster->tag;
			so->isPenetrate = true;
			so->skillName = L"AttackP";
			SceneManager::GetInstance().curScene->obm.AddObject(so);
		}
	}
}

void SkillAttack2::SpriteUpdate(float deltaTime)
{
	if (curState == State::kCharge)
		armSprite.curScene = bodySprite.curScene = 0;
	else if (curState == State::kCast)
		armSprite.curScene = bodySprite.curScene = 1;
	chargeSprite.Update(deltaTime);
}

void SkillAttack2::SpriteRender(const Sprite::RenderProperty& rp)
{
	auto _rp = rp;
	armSprite.Render(_rp);
	bodySprite.Render(_rp);

	if (curState == State::kCharge && chargeEff)
	{
		_rp.pos.y -= 20.0f;
		chargeSprite.Render(_rp);
	}
}
