#pragma once
class EditorScene : public Scene
{
public:
	enum Mode
	{
		TileMode,
		EnemyMode,
		Count
	};

	class TileEditor
	{
	public:
		int curIDX = 0;
		Tile tile;

		std::vector<std::wstring> tilePathes;
	} te;

	Mode mode = Mode::TileMode;
	TileMapManager::SpawnEnemy::Type curEnemyType = TileMapManager::SpawnEnemy::Type::Enemy1;
	int curLayer = 0;
	int curStep = -1;
	Tile::Type curType = Tile::Type::Empty;

public:
	virtual void Init();
	virtual void Term();
	virtual void Update(float deltaTime);
	virtual void FixedUpdate(float fixedDeltaTime);
	virtual void Render();
	virtual void UiRender();
};

