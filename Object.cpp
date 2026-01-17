#include "Core.h"

void Object::Update(float deltaTime)
{
}

void Object::LateUpdate(float deltaTime)
{
}

void Object::FixedUpdate(float fixedDeltaTime)
{
}

void Object::Render()
{
#ifdef _DEBUG

	auto& D3D = Direct3D::GetInstance();

	for (auto body : bodies)
	{
		if (body.type == Collider2D::Type::kAABB)
		{
			D3DXVECTOR2 min = body.aabb.min + position;
			D3DXVECTOR2 max = body.aabb.max + position;

			D3D.DrawLine(D3DXVECTOR2(min.x, min.y), D3DXVECTOR2(min.x, max.y));
			D3D.DrawLine(D3DXVECTOR2(min.x, max.y), D3DXVECTOR2(max.x, max.y));
			D3D.DrawLine(D3DXVECTOR2(max.x, max.y), D3DXVECTOR2(max.x, min.y));
			D3D.DrawLine(D3DXVECTOR2(max.x, min.y), D3DXVECTOR2(min.x, min.y));
		}
		else if (body.type == Collider2D::Type::kLine)
		{
			D3D.DrawLine(body.line.p1 + position, body.line.p2 + position);
		}
	}
#endif // _DEBUG

}

void Object::OnCollision(Collider2D& collision)
{
}

void ObjectManager::AddObject(std::shared_ptr<Object> obj)
{
	addObjects.push_back(obj);
}

void ObjectManager::AddUiObject(std::shared_ptr<Object> obj)
{
	uiAddObjects.push_back(obj);
}

void ObjectManager::Update(float deltaTime)
{
	objects.insert(objects.end(), addObjects.begin(), addObjects.end());
	addObjects.clear();
	uiObjects.insert(uiObjects.end(), uiAddObjects.begin(), uiAddObjects.end());
	uiAddObjects.clear();
	std::sort(objects.begin(), objects.end(), [](const std::shared_ptr<Object>& lhs, const std::shared_ptr<Object>& rhs)
		{
			return lhs->layer < rhs->layer;
		});

	for (auto iter : objects)
		iter->Update(deltaTime);
	for (auto iter : uiObjects)
		iter->Update(deltaTime);

	for (auto iter = objects.begin(); iter != objects.end();)
	{
		(*iter)->LateUpdate(deltaTime);
		if ((*iter)->isRemove)
			iter = objects.erase(iter);
		else
			++iter;
	}
	for (auto iter = uiObjects.begin(); iter != uiObjects.end();)
	{
		(*iter)->LateUpdate(deltaTime);
		if ((*iter)->isRemove)
			iter = uiObjects.erase(iter);
		else
			++iter;
	}
}

void ObjectManager::FixedUpdate(float fixedDeltaTime)
{
	for (auto iter = objects.begin(); iter != objects.end(); ++iter)
	{
		if (!(*iter)->isActiveCollider)
			continue;
		for (auto iter2 = iter + 1; iter2 != objects.end(); ++iter2)
		{
			if (!(*iter2)->isActiveCollider)
				continue;
			if ((*iter)->layer != (*iter2)->layer)
				continue;

			for (auto body : (*iter)->bodies)
			{
				for (auto body2 : (*iter2)->bodies)
				{
					if (IsCollisionObject(body, body2))
					{
						(*iter)->OnCollision(body2);
						(*iter2)->OnCollision(body);
					}
				}
			}
		}
	}

	for (auto iter : objects)
		iter->FixedUpdate(fixedDeltaTime);
	for (auto iter : uiObjects)
		iter->FixedUpdate(fixedDeltaTime);
}

void ObjectManager::Render()
{
	for (auto iter : objects)
		iter->Render();
}

void ObjectManager::UiRender()
{
	for (auto iter : uiObjects)
		iter->Render();
}
