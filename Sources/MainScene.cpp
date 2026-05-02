#include "Core.h"

void MainScene::Init()
{
	for (int i = 0; i < BtnType::kCount; ++i)
	{
		WCHAR buffer[255];
		swprintf_s(buffer, L"Assets/UI/Main/Buttons/%s/", btnNames[i].c_str());
		btn[i].reset(new Button);
		btn[i]->btnSprite.LoadAll(buffer);
		btn[i]->position = D3DXVECTOR2(0.0f, -40.0f * i);
		obm.AddUiObject(btn[i]);
	}

	btn[kStart]->OnClick = []()
	{
		SceneManager::GetInstance().AddScene(L"Stage1", std::shared_ptr<Scene>(new GameScene(1)));
		SceneManager::GetInstance().ChangeScene(L"Stage1");
	};

	btn[kHelp]->OnClick = [&]()
	{
		isShowHelp = true;
	};

	btn[kRank]->OnClick = []()
	{
		SceneManager::GetInstance().AddScene(L"Rank", std::shared_ptr<Scene>(new RankScene));
		SceneManager::GetInstance().ChangeScene(L"Rank");
	};

	btn[kExit]->OnClick = []()
	{
		PostQuitMessage(0);
	};

	Direct3D::GetInstance().camDestPos = Direct3D::GetInstance().camPos = D3DXVECTOR2(0.0f, 144.0f);
	Direct3D::GetInstance().camScale = Direct3D::GetInstance().camDestScale = D3DXVECTOR2(1.0f, 1.0f);

	titleSprite.LoadAll(L"Assets/UI/Main/Title.png");
	helpSprite.LoadAll(L"Assets/UI/Main/Help.png");

	TileMapManager::GetInstance().BindScene(SceneManager::GetInstance().curScene, L"Assets/Map/MainScene.txt");
}

void MainScene::Term()
{
}

void MainScene::Update(float deltaTime)
{
#ifdef _DEBUG
	if (KeyManager::GetInstance().GetKeyState('1') == KeyState::Down)
	{
		SceneManager::GetInstance().AddScene(L"Editor", std::shared_ptr<Scene>(new EditorScene));
		SceneManager::GetInstance().ChangeScene(L"Editor");
	}
#endif // _DEBUG

	if (isShowHelp)
	{
		for (int i = 0; i < BtnType::kCount; ++i)
		{
			btn[i]->eventHandler = false;
		}

		if (KeyManager::GetInstance().GetKeyState(VK_LBUTTON) == KeyState::Down)
		{
			isShowHelp = false;
		}
	}
	else
	{
		for (int i = 0; i < BtnType::kCount; ++i)
		{
			btn[i]->eventHandler = true;
		}
	}

	Direct3D::GetInstance().camPos = Direct3D::GetInstance().camDestPos = Direct3D::GetInstance().camPos + D3DXVECTOR2(5.0f, 0.0f) * deltaTime;
	if (Direct3D::GetInstance().camPos.x > Direct3D::GetInstance().resolutionWidth)
		Direct3D::GetInstance().camPos = Direct3D::GetInstance().camDestPos = D3DXVECTOR2(0.0f, 144.0f);

	Scene::Update(deltaTime);
}

void MainScene::FixedUpdate(float fixedDeltaTime)
{
	Scene::FixedUpdate(fixedDeltaTime);
}

void MainScene::Render()
{
	Scene::Render();
}

void MainScene::UiRender()
{
	Sprite::RenderProperty rp;
	titleSprite.Render(rp);

	Scene::UiRender();

	if (isShowHelp)
		helpSprite.Render(rp);
}
