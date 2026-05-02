#pragma once
#define TILEWIDTH 48
#define TILEHEIGHT 48

class Tile
{
public:
	enum class Type
	{
		Empty,
		Ground,
		Wall,
		Step
	};

	static const int typeCount = 4;
	static constexpr WCHAR* typeStr[typeCount] = {
		L"Empty",
		L"Ground",
		L"Wall",
		L"Step"
	};

	Type type = Type::Empty;
	POINT position = { 0, 0 };
	Sprite tileSprite;

	int step = -1;

public:
	void Write(std::wofstream& os);
	void Read(std::wifstream& is);
};

class TileMap : public Object
{
public:
	std::map<POINT, Tile> tiles;

public:
	void Update(float deltaTime);
	void Render();

	void AddTile(const Tile& tile);
	void DelTile(const POINT& pos);
	Tile* GetTile(const POINT& pos);
	bool IsEmpty();

	void Write(std::wofstream& os);
	void Read(std::wifstream& is);
};

class TileMapManager
{
private:
	TileMapManager() {};
	TileMapManager(const TileMapManager&) = delete;
	void operator =(const TileMapManager&) = delete;

public:
	static TileMapManager& GetInstance()
	{
		static TileMapManager instance;
		return instance;
	}

public:
	std::map<int, std::shared_ptr<TileMap>> tileMaps;

	class SpawnEnemy
	{
	public:
		enum Type
		{
			Enemy1,
			Enemy2,
			Enemy3,
			Boss1,
			Boss2,
			Count
		} type;
		static constexpr WCHAR* typeStrs[Type::Count] = {
			L"Enemy1",
			L"Enemy2",
			L"Enemy3",
			L"Boss1",
			L"Boss2"
		};
		D3DXVECTOR2 pos;
	};
	std::map<int, std::vector<SpawnEnemy>> spawnEnemiesInfo;
	std::vector<std::shared_ptr<Character>> spawnEnemies;

	int curStep = -1;

public:
	static D3DXVECTOR2 TP2WP(const POINT& tp);
	static POINT WP2TP(const D3DXVECTOR2& wp);

	void AddTile(const Tile& tile, ObjectManager& obm, const int& layer = 0);
	void DelTile(const POINT& pos, const int& layer = 0);
	Tile* GetTile(const int& layer, const POINT& pos);
	void BindScene(std::shared_ptr<Scene> trg, std::wstring path);
	void Write(std::wofstream& os);
	void Read(std::wifstream& is);
	void Reset();
	void EventUpdate(ObjectManager& obm);
};