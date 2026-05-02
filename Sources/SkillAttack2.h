#pragma once
class SkillAttack2 : public Skill
{
public:
	Sprite bodySprite;
	Sprite armSprite;
	Sprite chargeSprite;
	D3DXVECTOR2 chargePos;
	bool chargeEff = false;
	float chargeRatio = 0.0f;
	std::shared_ptr<Sound> attackSound;

public:
	SkillAttack2(Character* caster);

	virtual void OnLevelChanged(int level);
	virtual void OnCast(float ratio);
	virtual void OnCharge();
	virtual void OnExecute();
	virtual void OnCancel();
	virtual void OnUpdate(float fixedDeltaTime);
	virtual void SpriteUpdate(float deltaTime);
	virtual void SpriteRender(const Sprite::RenderProperty& rp);
};

