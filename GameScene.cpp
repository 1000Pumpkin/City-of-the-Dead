#include "Core.h"

GameScene::GameScene(int stage, const PlayerPref& pref)
{
	this->stage = stage;
	this->pref = pref;
}

void GameScene::Init()
{
	WCHAR buffer[255];
	swprintf(buffer, L"Assets/Map/Stage%d.txt", stage);
	TileMapManager::GetInstance().Reset();
	TileMapManager::GetInstance().BindScene(SceneManager::GetInstance().curScene, buffer);

	resultSprite.LoadAll(L"Assets/UI/Game/StageResult.png");
	resultProperty.a = 0.0f;

	tabSprite.LoadAll(L"Assets/UI/Game/PlayerSpecificGuide.png");
	tabProprety.pos = D3DXVECTOR2(-290.0f, -150.0f);

	swprintf_s(buffer, L"Assets/UI/Game/StageGuide/%d/Start/", stage);
	stageSprite[Start].LoadAll(buffer);
	swprintf_s(buffer, L"Assets/UI/Game/StageGuide/%d/Clear/", stage);
	stageSprite[Clear].LoadAll(buffer);
	swprintf_s(buffer, L"Assets/UI/Game/StageGuide/%d/Failed/", stage);
	stageSprite[Fail].LoadAll(buffer);
	for (int i = 0; i < 3; ++i)
		stageSprite[i].isLoop = false;

	player.reset(new Player());
	player->sp = pref.sp;
	player->level = pref.level;
	player->exp = pref.exp;
	obm.AddObject(player);

	score.reset(new Score());
	score->AddScore(pref.score);
	score->position = D3DXVECTOR2(200.0f, 200.0f);
	obm.AddUiObject(score);

	spec.reset(new PlayerSpec());
	for (int i = 0; i < PlayerSpec::Type::Count; ++i)
	{
		if (pref.isLearnType[i])
			spec->Learn((PlayerSpec::Type)i, player);
	}
	obm.AddUiObject(spec);

	obm.AddUiObject(std::shared_ptr<Object>(new PlayerInfo));

	obm.AddUiObject(std::shared_ptr<Object>(new ComboUI));

	bgmSound.reset(new Sound(L"Assets/SoundFX/BGM.mp3", 1));
	bossBgmSound.reset(new Sound(L"Assets/SoundFX/BossBGM.mp3", 1));

	bgmSound->Play(true);

	Direct3D::GetInstance().camDestPos = Direct3D::GetInstance().camPos = D3DXVECTOR2(0.0f, 0.0f);
	Direct3D::GetInstance().camDestScale = Direct3D::GetInstance().camScale = D3DXVECTOR2(1.0f, 1.0f);
}

void GameScene::Update(float deltaTime)
{
	for (int i = 0; i < PlayerSpec::Type::Count; ++i)
	{
		pref.isLearnType[i] = spec->isLearnType[i];
		pref.level = player->level;
		pref.score = score->score;
		pref.exp = player->exp;
		pref.sp = player->sp;
	}

	if (player->sp != 0)
	{
		tabProprety.a += deltaTime;
		if (tabProprety.a >= 1.0f)
			tabProprety.a = 0.0f;
	}
	else
	{
		tabProprety.a = 0.0f;
	}

	if (KeyManager::GetInstance().GetKeyState(VK_TAB) == KeyState::Down)
	{
		spec->isDisplay = !spec->isDisplay;
	}

	if (clear)
	{
		bossBgmSound->Stop();
		player->isActiveCollider = false;
		state = StageState::Clear;
	}
	else if (player->bodyState == Character::State::kDead)
	{
		state = StageState::Fail;
	}

	if (stageSprite[state].Update(deltaTime))
	{
		isAniEnd = true;
		if (state == StageState::Start)
		{
			stageProperty[state].a = 0.0f;
		}
	}

	if (isAniEnd)
	{
		if (state == StageState::Fail)
		{
			if (KeyManager::GetInstance().GetKeyState(VK_LBUTTON) == KeyState::Down)
			{
				bgmSound->Stop();
				bossBgmSound->Stop();
				SceneManager::GetInstance().AddScene(L"Main", std::shared_ptr<Scene>(new MainScene));
				SceneManager::GetInstance().ChangeScene(L"Main");
			}
		}
		if (state == StageState::Clear)
		{
			resultProperty.a += deltaTime * 0.5f;
			resultTime += deltaTime;
			if (resultProperty.a >= 1.0f)
			{
				resultProperty.a = 1.0f;
				if (resultTime >= 3.0f)
				{
					bgmSound->Stop();
					bossBgmSound->Stop();
					int hpScore = (int)(((float)player->hp / (float)player->maxHp) * 100000);
					pref.score += hpScore;
					if (stage + 1 > 2)
					{
						SceneManager::GetInstance().AddScene(L"Rank", std::shared_ptr<Scene>(new RankScene(pref.score)));
						SceneManager::GetInstance().ChangeScene(L"Rank");
					}
					else
					{
						SceneManager::GetInstance().AddScene(L"Stage2", std::shared_ptr<Scene>(new GameScene(2, pref)));
						SceneManager::GetInstance().ChangeScene(L"Stage2");
					}
				}
			}
		}
	}

	if (KeyManager::GetInstance().GetKeyState(VK_F1) == KeyState::Down)
	{
		player->isActiveCollider = !player->isActiveCollider;
	}
	if (KeyManager::GetInstance().GetKeyState(VK_F2) == KeyState::Down)
	{
		++player->sp;
	}
	if (KeyManager::GetInstance().GetKeyState(VK_F3) == KeyState::Down)
	{
		player->exp += player->maxExp;
	}
	if (KeyManager::GetInstance().GetKeyState(VK_F4) == KeyState::Down)
	{
		SceneManager::GetInstance().AddScene(L"Main", std::shared_ptr<Scene>(new MainScene));
		SceneManager::GetInstance().ChangeScene(L"Main");
		bgmSound->Stop();
		bossBgmSound->Stop();
	}

	if (KeyManager::GetInstance().GetKeyState(VK_F5) == KeyState::Down)
	{
		SceneManager::GetInstance().AddScene(L"Stage1", std::shared_ptr<Scene>(new GameScene(1, pref)));
		SceneManager::GetInstance().ChangeScene(L"Stage1");
		bgmSound->Stop();
		bossBgmSound->Stop();
	}
	if (KeyManager::GetInstance().GetKeyState(VK_F6) == KeyState::Down)
	{
		SceneManager::GetInstance().AddScene(L"Stage2", std::shared_ptr<Scene>(new GameScene(2, pref)));
		SceneManager::GetInstance().ChangeScene(L"Stage2");
		bgmSound->Stop();
		bossBgmSound->Stop();
	}
	if (KeyManager::GetInstance().GetKeyState(VK_F7) == KeyState::Down)
	{
		player->damageRate = 1000.0f;
	}
	if (KeyManager::GetInstance().GetKeyState(VK_F8) == KeyState::Down)
	{
		obm.AddObject(std::shared_ptr<Object>(new Item(rand() % 2, player->position + D3DXVECTOR2(0.0f, 50.0f))));
	}
	Scene::Update(deltaTime);
}

void GameScene::FixedUpdate(float fixedDeltaTime)
{
	TileMapManager::GetInstance().EventUpdate(obm);
	Scene::FixedUpdate(fixedDeltaTime);
}

void GameScene::Render()
{
	Scene::Render();
}

void GameScene::UiRender()
{
	Scene::UiRender();

	stageSprite[state].Render(stageProperty[state]);

	if (player->sp != 0)
	{
		tabSprite.Render(tabProprety);
	}

	resultSprite.Render(resultProperty);

	if (clear)
	{
		Number num;
		num.rp.a = resultProperty.a;
		num.LoadAll(L"Assets/UI/Number/", 8);
		num.SetScale(D3DXVECTOR2(1.0f, 1.0f));
		num.SetNumber(score->score);
		num.position = D3DXVECTOR2(-90.0f, 70.0f);
		num.Render();
		int hpScore = (int)(((float)player->hp / (float)player->maxHp) * 100000);
		num.SetNumber(hpScore);
		num.position.y -= 50.0f;
		num.Render();
		num.SetNumber(score->score + hpScore);
		num.position.y -= 50.0f;
		num.Render();
	}
}
