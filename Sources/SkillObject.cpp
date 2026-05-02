#include "Core.h"

void SkillObject::Update(float deltaTime)
{
	soSprite.Update(deltaTime);
}

void SkillObject::LateUpdate(float deltaTime)
{
}

void SkillObject::FixedUpdate(float fixedDeltaTime)
{
	if (target)
	{
		auto dp = target->position - position;
		if (dp != D3DXVECTOR2(0.0f, 0.0f))
		{
			D3DXVec2Normalize(&dp, &dp);
			position += dp * moveSpeed * fixedDeltaTime;
		}
	}
	else
	{
		position += moveDir * moveSpeed * fixedDeltaTime;
	}

	if (isActiveCollider)
	{
		if (moveDir != D3DXVECTOR2(0.0f, 0.0f))
		{
			auto nextTile = TileMapManager::GetInstance().GetTile(0, TileMapManager::GetInstance().WP2TP(position));
			if (nextTile)
			{
				if (nextTile->type == Tile::Type::Wall)
					isRemove = true;
			}
		}
	}

	if (duration < 0.0f)
		isRemove = true;

	duration -= fixedDeltaTime;
}

void SkillObject::Render()
{
	if (soSprite.maxScene != 0)
	{
		rp.pos = position;
		soSprite.Render(rp);
	}
	Object::Render();
}

void SkillObject::OnCollision(Collider2D& collision)
{
	if (isRemove)
		return;

	if (collision.object->tag != tag)
	{
		if (isPenetrate)
		{
			if (std::find(hitters.begin(), hitters.end(), collision.object) != hitters.end())
				return;
		}
		if (auto character = dynamic_cast<Character*>(collision.object))
		{
			if (!isPenetrate)
				isRemove = true;

			if (hitEffSprite.maxScene != 0)
			{
				std::shared_ptr<Effect> hitEff(new Effect(hitEffSprite.filePath));
				hitEff->position = character->position;
				SceneManager::GetInstance().curScene->obm.AddObject(hitEff);
			}
			if (collision.object->tag == L"Enemy")
			{
				auto player = std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->player;
				++player->comboCount;
			}
			hitters.push_back(collision.object);
			character->OnHit(damage, elasticForce, hitDuration, skillName);
		}
	}
}
