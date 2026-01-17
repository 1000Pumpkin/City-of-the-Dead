#include "Core.h"

bool operator < (const POINT& a, const POINT& b)
{
	return a.x < b.x || (a.x == b.x && a.y < b.y);
}

void Tile::Write(std::wofstream& os)
{
	os << (int&)type << std::endl;
	os << position.x << std::endl;
	os << position.y << std::endl;
	os << tileSprite.filePath << std::endl;
	os << step << std::endl;
}

void Tile::Read(std::wifstream& is)
{
	is >> (int&)type;
	is >> position.x;
	is >> position.y;
	std::wstring filePath;
	is >> filePath;
	tileSprite.LoadAll(filePath);
	is >> step;
}

void TileMap::Update(float deltaTime)
{
	for (auto& tile : tiles)
	{
		tile.second.tileSprite.Update(deltaTime);
	}
}

void TileMap::Render()
{
	Sprite::RenderProperty rp;
#ifdef _DEBUG
	if (std::dynamic_pointer_cast<EditorScene>(SceneManager::GetInstance().curScene) == NULL)
#endif // _DEBUG
		if (layer == 0)
		{
			for (int y = -10; y < 10; ++y)
			{
				for (int x = -20; x < 20; ++x)
				{
					auto pp = TileMapManager::GetInstance().WP2TP(Direct3D::GetInstance().camPos);
					if (auto tile = TileMapManager::GetInstance().GetTile(layer, POINT{ pp.x + x, pp.y + y }))
					{
						auto wp = TileMapManager::GetInstance().TP2WP(tile->position) + position;
						rp.pos = wp;
						tile->tileSprite.Render(rp);
					}
				}
			}
			return;
		}

	for (auto& tile : tiles)
	{
#ifdef _DEBUG
		if (std::dynamic_pointer_cast<EditorScene>(SceneManager::GetInstance().curScene) == NULL)
#endif // _DEBUG
			if (!TileMapManager::GetInstance().GetTile(layer, tile.second.position))
				continue;
		D3DXVECTOR2 curPos = TileMapManager::GetInstance().TP2WP(tile.second.position) + position;
		curPos += Direct3D::GetInstance().camPos * -(float)layer * 0.05f;
		rp.pos = curPos;
		tile.second.tileSprite.Render(rp);
	}
}

void TileMap::AddTile(const Tile& tile)
{
	tiles[tile.position] = tile;
}

void TileMap::DelTile(const POINT& pos)
{
	auto tf = tiles.find(pos);
	if (tf != tiles.end())
		tiles.erase(pos);
}

Tile* TileMap::GetTile(const POINT& pos)
{
	auto tf = tiles.find(pos);
	if (tf != tiles.end())
		return &tf->second;
	return nullptr;
}

bool TileMap::IsEmpty()
{
	return tiles.size() == 0;
}

void TileMap::Write(std::wofstream& os)
{
	os << layer << std::endl;
	os << tiles.size() << std::endl;
	for (auto& tile : tiles)
		tile.second.Write(os);
}

void TileMap::Read(std::wifstream& is)
{
	is >> layer;
	int szTile = 0;
	is >> szTile;
	for (int i = 0; i < szTile; ++i)
	{
		Tile tile;
		tile.Read(is);
		tiles.insert(std::make_pair(tile.position, tile));
	}
}

D3DXVECTOR2 TileMapManager::TP2WP(const POINT& tp)
{
	return D3DXVECTOR2(tp.x * (float)TILEWIDTH, tp.y * (float)TILEHEIGHT);
}

POINT TileMapManager::WP2TP(const D3DXVECTOR2& wp)
{
	POINT tp;

	if (wp.x < 0.0f)
		tp.x = (int)((wp.x - TILEWIDTH * 0.5f) / TILEWIDTH);
	else
		tp.x = (int)((wp.x + TILEWIDTH * 0.5f) / TILEWIDTH);

	if (wp.y < 0.0f)
		tp.y = (int)((wp.y - TILEHEIGHT * 0.5f) / TILEHEIGHT);
	else
		tp.y = (int)((wp.y + TILEHEIGHT * 0.5f) / TILEHEIGHT);

	return tp;
}

void TileMapManager::AddTile(const Tile& tile, ObjectManager& obm, const int& layer)
{
	auto& tm = tileMaps[layer];
	if (tm == NULL)
	{
		tm.reset(new TileMap());
		tm->layer = layer;
		obm.AddObject(tm);
	}
	tm->AddTile(tile);
}

void TileMapManager::DelTile(const POINT& pos, const int& layer)
{
	auto tf = tileMaps.find(layer);
	if (tf == tileMaps.end())
		return;

	tf->second->DelTile(pos);
	if (tf->second->IsEmpty())
	{
		tf->second->isRemove = true;
		tileMaps.erase(layer);
	}
}

Tile* TileMapManager::GetTile(const int& layer, const POINT& pos)
{
	auto tf = tileMaps.find(layer);
	if (tf != tileMaps.end())
	{
		auto tile = tf->second->GetTile(pos);
		if (tile)
		{
			if (tile->step == -1)
				return tile;
			if (tile->step == curStep)
				return tile;
			if (tile->type == Tile::Type::Wall && tile->step < curStep - 2)
				return tile;
		}
	}
	return nullptr;
}

void TileMapManager::BindScene(std::shared_ptr<Scene> trg, std::wstring path)
{
	std::wifstream is;
	is.open(path);
	
	if (!is.is_open())
	{
		wprintf(L"Can't Find Path : %s", path.c_str());
		return;
	}

	Read(is);
	for (auto& tileMap : tileMaps)
	{
		trg->obm.AddObject(tileMap.second);
	}
}

void TileMapManager::Write(std::wofstream& os)
{
	os << tileMaps.size() << std::endl;
	for (auto& tileMap : tileMaps)
		tileMap.second->Write(os);

	os << spawnEnemiesInfo.size() << std::endl;
	for (auto& info : spawnEnemiesInfo)
	{
		os << info.first << std::endl;
		os << info.second.size() << std::endl;
		for (auto& info2 : info.second)
		{
			os << info2.type << std::endl;
			os << info2.pos.x << std::endl;
			os << info2.pos.y << std::endl;
		}
	}
}

void TileMapManager::Read(std::wifstream& is)
{
	tileMaps.clear();

	int szTileMap = 0;
	is >> szTileMap;
	for (int i = 0; i < szTileMap; ++i)
	{
		std::shared_ptr<TileMap> tileMap(new TileMap());
		tileMap->Read(is);
		tileMaps.insert(std::make_pair(tileMap->layer, tileMap));
	}

	spawnEnemiesInfo.clear();
	int szSpawnEnemiesInfo = 0;
	is >> szSpawnEnemiesInfo;
	for (int i = 0; i < szSpawnEnemiesInfo; ++i)
	{
		int step, szInfos;
		is >> step;
		is >> szInfos;
		for (int j = 0; j < szInfos; ++j)
		{
			SpawnEnemy se;
			is >> (int&)se.type;
			is >> se.pos.x;
			is >> se.pos.y;
			spawnEnemiesInfo[step].push_back(se);
		}
	}
}

void TileMapManager::Reset()
{
	curStep = 0;
	tileMaps.clear();
	spawnEnemies.clear();
}

void TileMapManager::EventUpdate(ObjectManager& obm)
{
	if (!spawnEnemies.empty())
	{
		bool allDead = true;
		for (auto& enemy : spawnEnemies)
		{
			if (enemy->bodyState != Character::State::kDead)
				allDead = false;
		}
		if (allDead)
		{
			spawnEnemies.clear();
			++curStep;
		}
		return;
	}

	auto tile = GetTile(0, WP2TP(std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->player->position));
	if (tile)
	{
		if (tile->type == Tile::Type::Step)
		{
			++curStep;
		}
	}

	auto sf = spawnEnemiesInfo.find(curStep);
	if (sf != spawnEnemiesInfo.end())
	{
		for (auto se : sf->second)
		{
			std::shared_ptr<Character> enemy = NULL;
			switch (se.type)
			{
			case SpawnEnemy::Type::Enemy1:
				enemy.reset(new Enemy1);
				break;
			case SpawnEnemy::Type::Enemy2:
				enemy.reset(new Enemy2);
				break;
			case SpawnEnemy::Type::Enemy3:
				enemy.reset(new Enemy3);
				break;
			case SpawnEnemy::Type::Boss1:
				enemy.reset(new Boss1);
				break;
			case SpawnEnemy::Type::Boss2:
				enemy.reset(new Boss2);
				break;
			}

			if (enemy)
			{
				enemy->position = se.pos;
				spawnEnemies.push_back(enemy);
				obm.AddObject(enemy);
			}
		}
	}
}
