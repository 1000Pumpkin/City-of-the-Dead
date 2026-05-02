#pragma once
class SkillBoss1 : public Skill
{
private:
	Sprite sprite;
	Sprite bulletSprite;
	int spawnBullet;
	std::vector<std::shared_ptr<SkillObject>> bullets;
	float spawnMinRotate = -70.0f;
	float spawnMaxRotate = 70.0f;

public:
	SkillBoss1(Character* caster, int spawnBullet);

	virtual void OnLevelChanged(int level);
	virtual void OnCast(float ratio);
	virtual void OnCharge();
	virtual void OnExecute();
	virtual void OnCancel();
	virtual void OnUpdate(float fixedDeltaTime);
	virtual void SpriteUpdate(float deltaTime);
	virtual void SpriteRender(const Sprite::RenderProperty& rp);
};

