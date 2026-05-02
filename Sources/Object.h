#pragma once

class Object
{
public:
	int layer = 0;
	std::wstring tag = L"";
	D3DXVECTOR2 position = D3DXVECTOR2(0.0f, 0.0f);
	std::vector<Collider2D> bodies;
	bool isRemove = false;
	bool isActiveCollider = true;

public:
	virtual ~Object() {};
	virtual void Update(float deltaTime);
	virtual void LateUpdate(float deltaTime);
	virtual void FixedUpdate(float fixedDeltaTime);
	virtual void Render();
	virtual void OnCollision(Collider2D& collision);
};

class ObjectManager
{
private:
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<std::shared_ptr<Object>> addObjects;
	std::vector<std::shared_ptr<Object>> uiObjects;
	std::vector<std::shared_ptr<Object>> uiAddObjects;
	
public:
	void AddObject(std::shared_ptr<Object> obj);
	void AddUiObject(std::shared_ptr<Object> obj);

	void Update(float deltaTime);
	void FixedUpdate(float fixedDeltaTime);
	void Render();
	void UiRender();
};