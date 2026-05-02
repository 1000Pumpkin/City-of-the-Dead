#pragma once
class SkillBoss2 : public Skill
{
private:
	Sprite addonSprite;
	int spawnAddon;
	float spawnMinRotate = -70.0f;
	float spawnMaxRotate = 70.0f;

public:
	SkillBoss2(Character* caster, int spawnAddon);

	virtual void OnLevelChanged(int level);
	virtual void OnCast(float ratio);
	virtual void OnCharge();
	virtual void OnExecute();
	virtual void OnCancel();
	virtual void OnUpdate(float fixedDeltaTime);
	virtual void SpriteUpdate(float deltaTime);
	virtual void SpriteRender(const Sprite::RenderProperty& rp);
};

