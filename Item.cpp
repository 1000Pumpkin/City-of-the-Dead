#include "Core.h"

Item::Item(int type, D3DXVECTOR2 pos)
{
	this->type = (Type)type;

	if (this->type == Type::Hp)
	{
		tag = L"Hp";
	}
	else if (this->type == Type::Exp)
	{
		tag = L"Exp";
	}

	sprite.LoadAll(L"Assets/Item/");
	sprite.curScene = type;
	position = pos;

	Collider2D::AABB aabb;
	aabb.min = D3DXVECTOR2(-10.0f, -10.0f);
	aabb.max = D3DXVECTOR2(10.0f, 10.0f);
	bodies.push_back(Collider2D(this, &aabb, nullptr, nullptr));
}

void Item::Update(float deltaTime)
{
}

void Item::LateUpdate(float deltaTime)
{
}

void Item::FixedUpdate(float fixedDeltaTime)
{
	aliveTime += fixedDeltaTime;

	if (aliveTime >= 10.0f)
	{
		rp.a -= fixedDeltaTime;
		if (rp.a <= 0.0f)
			isRemove = true;
	}
	vSpeed -= 300.0f * fixedDeltaTime;
	position.y += vSpeed * fixedDeltaTime;

	auto nextTile = TileMapManager::GetInstance().GetTile(0, TileMapManager::GetInstance().WP2TP(position));
	if (nextTile)
	{
		if (nextTile->type == Tile::Type::Ground)
		{
			if (vSpeed < 0.0f)
			{
				position.y = TileMapManager::GetInstance().TP2WP(nextTile->position).y + TILEHEIGHT * 0.5f + (vSpeed * fixedDeltaTime * 0.5f);
				defaultVSpeed *= 0.8f;
				vSpeed = defaultVSpeed;
			}
		}
		else if (nextTile->type == Tile::Type::Wall)
		{
			if (vSpeed < 0.0f)
			{
				position.y = TileMapManager::GetInstance().TP2WP(nextTile->position).y + TILEHEIGHT * 0.5f + 0.1f;
				defaultVSpeed *= 0.8f;
				vSpeed = defaultVSpeed;
			}
			else if (vSpeed > 0.0f)
			{
				position.y = TileMapManager::GetInstance().TP2WP(nextTile->position).y + TILEHEIGHT * 0.5f + 0.1f;
				defaultVSpeed *= 0.8f;
				vSpeed = defaultVSpeed;
			}
		}
	}
}

void Item::Render()
{
	rp.pos = position;
	sprite.Render(rp);
	Object::Render();
}

void Item::OnCollision(Collider2D& collision)
{
}
