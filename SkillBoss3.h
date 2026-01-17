#pragma once
class SkillBoss3 : public Skill
{
private:
	Sprite sprite;
	int shotBullet;
	float spawnMinRotate = -70.0f;
	float spawnMaxRotate = 70.0f;

public:
	SkillBoss3(Character* caster, int shotBullet);

	virtual void OnLevelChanged(int level);
	virtual void OnCast(float ratio);
	virtual void OnCharge();
	virtual void OnExecute();
	virtual void OnCancel();
	virtual void OnUpdate(float fixedDeltaTime);
	virtual void SpriteUpdate(float deltaTime);
	virtual void SpriteRender(const Sprite::RenderProperty& rp);
};

