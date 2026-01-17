#pragma once
class SkillEnemy2 : public Skill
{
public:
	Sprite sprite;
	std::wstring bulletPath;

public:
	SkillEnemy2(Character* caster, std::wstring path);

	virtual void OnLevelChanged(int level);
	virtual void OnCast(float ratio);
	virtual void OnCharge();
	virtual void OnExecute();
	virtual void OnCancel();
	virtual void OnUpdate(float fixedDeltaTime);
	virtual void SpriteUpdate(float deltaTime);
	virtual void SpriteRender(const Sprite::RenderProperty& rp);
};

