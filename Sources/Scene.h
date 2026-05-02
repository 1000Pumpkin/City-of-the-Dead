#pragma once

class Scene
{
public:
	ObjectManager obm;

public:
	virtual ~Scene() {};
	virtual void Init();
	virtual void Term();
	virtual void Update(float deltaTime);
	virtual void FixedUpdate(float fixedDeltaTime);
	virtual void Render();
	virtual void UiRender();
};

class SceneManager
{
private:
	SceneManager();
	SceneManager(const SceneManager&) = delete;
	void operator =(const SceneManager&) = delete;

public:
	static SceneManager& GetInstance()
	{
		static SceneManager instance;
		return instance;
	}

public:
	Sprite ldSprite;
	Sprite::RenderProperty ldProperty;
	std::map<std::wstring, std::shared_ptr<Scene>> sceneMap;
	std::shared_ptr<Scene> curScene, nextScene;

public:
	void AddScene(std::wstring name, std::shared_ptr<Scene> scene);
	void RemoveScene(std::wstring name);
	void ChangeScene(std::wstring name);

	void Update(float deltaTime);
	void FixedUpdate(float fixedDeltaTime);
	void Render();
	void UiRender();
};