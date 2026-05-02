#include "Core.h"

void EditorScene::Init()
{
	static const std::wstring tileDirPath = L"Assets/Map/";

	for (auto& dir : std::filesystem::recursive_directory_iterator(tileDirPath))
	{
		if (std::filesystem::is_directory(dir))
			te.tilePathes.push_back(dir.path());
	}

	std::wstring path;
	std::wcin >> path;
	TileMapManager::GetInstance().Reset();
	TileMapManager::GetInstance().BindScene(SceneManager::GetInstance().curScene, path);

	te.tile.tileSprite.LoadAll(te.tilePathes[te.curIDX]);
}

void EditorScene::Term()
{
}

void EditorScene::Update(float deltaTime)
{
	D3DXVECTOR2 camPos = Direct3D::GetInstance().camPos;
	D3DXVECTOR2 camScale = Direct3D::GetInstance().camScale;

	if (KeyManager::GetInstance().GetKeyState(VK_LCONTROL) == KeyState::None)
	{
		if (KeyManager::GetInstance().GetKeyState('A'))
			camPos.x -= 100.0f;
		if (KeyManager::GetInstance().GetKeyState('D'))
			camPos.x += 100.0f;
		if (KeyManager::GetInstance().GetKeyState('S'))
			camPos.y -= 100.0f;
		if (KeyManager::GetInstance().GetKeyState('W'))
			camPos.y += 100.0f;

		if (KeyManager::GetInstance().GetKeyState('Q') == KeyState::Down)
		{
			camScale.x += 0.2f;
			camScale.y += 0.2f;
		}
		if (KeyManager::GetInstance().GetKeyState('E') == KeyState::Down)
		{
			camScale.x -= 0.2f;
			camScale.y -= 0.2f;
		}
	}
	if (camPos != Direct3D::GetInstance().camPos)
	{
		Direct3D::GetInstance().camDestPos = camPos;
	}
	if (camScale != Direct3D::GetInstance().camScale)
	{
		Direct3D::GetInstance().camDestScale = camScale;
	}

	if (KeyManager::GetInstance().GetKeyState(VK_TAB) == KeyState::Down)
	{
		if (++(int&)mode >= Mode::Count)
			(int&)mode = 0;
	}

	if (mode == Mode::TileMode)
	{
		te.tile.position = TileMapManager::WP2TP(KeyManager::GetInstance().worldCursorPos);

		if (KeyManager::GetInstance().GetKeyState('1') == KeyState::Down)
		{
			if (++te.curIDX >= (int)te.tilePathes.size())
				te.curIDX = 0;
			te.tile.tileSprite.LoadAll(te.tilePathes[te.curIDX]);
		}
		if (KeyManager::GetInstance().GetKeyState('2') == KeyState::Down)
		{
			if (--te.curIDX < 0)
				te.curIDX = te.tilePathes.size() - 1;;
			te.tile.tileSprite.LoadAll(te.tilePathes[te.curIDX]);
		}
		if (KeyManager::GetInstance().GetKeyState('5') == KeyState::Down)
		{
			if (++(int&)curType > (int)Tile::Type::Step)
				curType = Tile::Type::Empty;
		}

		if (KeyManager::GetInstance().GetKeyState(VK_LBUTTON))
		{
			te.tile.type = curType;
			te.tile.step = curStep;
			te.tile.tileSprite.LoadAll(te.tilePathes[te.curIDX]);
			TileMapManager::GetInstance().AddTile(te.tile, obm, curLayer);
		}
		if (KeyManager::GetInstance().GetKeyState(VK_RBUTTON))
		{
			TileMapManager::GetInstance().DelTile(te.tile.position, curLayer);
		}
	}
	else if (mode == Mode::EnemyMode)
	{
		if (KeyManager::GetInstance().GetKeyState('1') == KeyState::Down)
		{
			if (++(int&)curEnemyType >= TileMapManager::SpawnEnemy::Type::Count)
				(int&)curEnemyType = 0;
		}
		if (KeyManager::GetInstance().GetKeyState('2') == KeyState::Down)
		{
			if (--(int&)curEnemyType < 0)
				(int&)curEnemyType = TileMapManager::SpawnEnemy::Type::		Count - 1;
		}
		if (KeyManager::GetInstance().GetKeyState(VK_DELETE) == KeyState::Down)
		{
			TileMapManager::GetInstance().spawnEnemiesInfo[curStep].clear();
		}
		if (KeyManager::GetInstance().GetKeyState(VK_LBUTTON) == KeyState::Down)
		{
			TileMapManager::SpawnEnemy se;
			se.type = curEnemyType;
			se.pos = KeyManager::GetInstance().worldCursorPos;
			TileMapManager::GetInstance().spawnEnemiesInfo[curStep].push_back(se);
		}
	}

	if (KeyManager::GetInstance().GetKeyState('3') == KeyState::Down)
		--curLayer;
	if (KeyManager::GetInstance().GetKeyState('4') == KeyState::Down)
		++curLayer;

	if (KeyManager::GetInstance().GetKeyState(VK_INSERT) == KeyState::Down)
		--curStep;
	if (KeyManager::GetInstance().GetKeyState(VK_HOME) == KeyState::Down)
		++curStep;

	if (KeyManager::GetInstance().GetKeyState(VK_LCONTROL))
	{
		if (KeyManager::GetInstance().GetKeyState('S') == KeyState::Down)
		{
			std::wstring path;
			std::wcin >> path;
			std::wofstream os;
			os.open(path);
			TileMapManager::GetInstance().Write(os);
		}
	}

	Scene::Update(deltaTime);
	te.tile.tileSprite.Update(deltaTime);
}

void EditorScene::FixedUpdate(float fixedDeltaTime)
{
	Scene::FixedUpdate(fixedDeltaTime);
}

void EditorScene::Render()
{
	Scene::Render();
	WCHAR buffer[255];
	auto pFont = Direct3D::GetInstance().pFont;

	if (mode == Mode::TileMode)
	{
		swprintf_s(buffer, L"TilePath : %s\nLayer : %d\nTileType : %s\nStep : %d", te.tilePathes[te.curIDX].c_str(), curLayer, Tile::typeStr[(int&)curType], curStep);
		Sprite::RenderProperty rp;
		rp.pos = TileMapManager::GetInstance().TP2WP(te.tile.position);
		te.tile.tileSprite.Render(rp);
	}
	else if (mode == Mode::EnemyMode)
	{
		swprintf_s(buffer, L"EnemyType : %s\nStep : %d", TileMapManager::SpawnEnemy::typeStrs[curEnemyType], curStep);
	}
	RECT rc = { 0.0f, 30.0f, 0.0f, 0.0f };
	pFont->DrawTextW(NULL, buffer, -1, &rc, DT_NOCLIP, 0xffffffff);
}

void EditorScene::UiRender()
{
	Scene::UiRender();
}
