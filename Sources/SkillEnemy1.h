#pragma once
class SkillEnemy1 : public Skill
{
public:
	Sprite sprite;

public:
	SkillEnemy1(Character* caster);

	virtual void OnLevelChanged(int level);
	virtual void OnCast(float ratio);
	virtual void OnCharge();
	virtual void OnExecute();
	virtual void OnCancel();
	virtual void OnUpdate(float fixedDeltaTime);
	virtual void SpriteUpdate(float deltaTime);
	virtual void SpriteRender(const Sprite::RenderProperty& rp);
};

