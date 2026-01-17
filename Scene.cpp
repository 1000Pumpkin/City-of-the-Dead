#include "Core.h"

void Scene::Init()
{
}

void Scene::Term()
{
}

void Scene::Update(float deltaTime)
{
	obm.Update(deltaTime);
}

void Scene::FixedUpdate(float fixedDeltaTime)
{
	obm.FixedUpdate(fixedDeltaTime);
}

void Scene::Render()
{
	obm.Render();
}

void Scene::UiRender()
{
	obm.UiRender();
}

SceneManager::SceneManager()
{
	ldSprite.LoadAll(L"Assets/UI/Loading/");
}

void SceneManager::AddScene(std::wstring name, std::shared_ptr<Scene> scene)
{
	RemoveScene(name);
	auto sf = sceneMap.find(name);
	if (sf != sceneMap.end())
		return;

	sceneMap.insert(std::make_pair(name, scene));
}

void SceneManager::RemoveScene(std::wstring name)
{
	auto sf = sceneMap.find(name);
	if (sf != sceneMap.end())
	{
		sf->second->Term();
		sceneMap.erase(sf);
	}
}

void SceneManager::ChangeScene(std::wstring name)
{
	if (nextScene)
		return;
	auto sf = sceneMap.find(name);
	if (sf != sceneMap.end())
		nextScene = sf->second;
}

void SceneManager::Update(float deltaTime)
{
	if (nextScene)
	{
		ldProperty.a += deltaTime;
		if (ldProperty.a >= 1.0f)
		{
			ldProperty.a = 1.0f;
			curScene = nextScene;
			curScene->Init();
			nextScene = NULL;
		}
	}
	else
	{
		ldProperty.a -= deltaTime;
		if (ldProperty.a <= 0.0f)
			ldProperty.a = 0.0f;
	}

	if (curScene)
		curScene->Update(deltaTime);
}

void SceneManager::FixedUpdate(float fixedDeltaTime)
{
	if (curScene)
		curScene->FixedUpdate(fixedDeltaTime);
}

void SceneManager::Render()
{
	if (curScene)
		curScene->Render();
}

void SceneManager::UiRender()
{
	if (curScene)
		curScene->UiRender();
	ldSprite.Render(ldProperty);
}
