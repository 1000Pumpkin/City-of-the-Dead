#pragma once

class Object;

class Collider2D
{
public:
	enum Type
	{
		kAABB,
		kCircle,
		kLine
	} type;

	class AABB
	{
	public:
		D3DXVECTOR2 min = D3DXVECTOR2(0.0f, 0.0f);
		D3DXVECTOR2 max = D3DXVECTOR2(0.0f, 0.0f);
	} aabb;

	class Circle
	{
	public:
		float radius = 0.0f;
	} circle;

	class Line
	{
	public:
		D3DXVECTOR2 p1 = D3DXVECTOR2(0.0f, 0.0f);
		D3DXVECTOR2 p2 = D3DXVECTOR2(0.0f, 0.0f);
	} line;

	Object* object = nullptr;

public:
	Collider2D(Object* obj, AABB* a, Circle* c, Line* l);
};

bool IsCollisionObject(const Collider2D& col1, const Collider2D& col2);
bool IsCollisionAABB(const D3DXVECTOR2& min1, const D3DXVECTOR2& max1, const D3DXVECTOR2& min2, const D3DXVECTOR2& max2);
bool IsCollisionAABBPos(const D3DXVECTOR2& min, const D3DXVECTOR2& max, const D3DXVECTOR2& pos);
bool IsCollisionCircle(const D3DXVECTOR2& p1, const float& r1, const D3DXVECTOR2& p2, const float& r2);
bool IsCollisionLine(const D3DXVECTOR2& p1, const D3DXVECTOR2& p2, const D3DXVECTOR2& p3, const D3DXVECTOR2& p4);
float D3DXVec2Angle(const D3DXVECTOR2& dest, const D3DXVECTOR2& src = D3DXVECTOR2(0.0f, 1.0f));
D3DXVECTOR2 D3DXVec2Rotation(const D3DXVECTOR2& vec, const float& rotate);