#include "Core.h"

Collider2D::Collider2D(Object* obj, AABB* a, Circle* c, Line* l)
{
	object = obj;

	if (a)
	{
		type = Type::kAABB;
		aabb = *a;
	}
	else if (c)
	{
		type = Type::kCircle;
		circle = *c;
	}
	else if (l)
	{
		type = Type::kLine;
		line = *l;
	}
}

bool IsCollisionObject(const Collider2D& col1, const Collider2D& col2)
{
	auto col1Pos = col1.object->position;
	auto col2Pos = col2.object->position;

	if (col1.type == Collider2D::Type::kAABB && col2.type == Collider2D::Type::kAABB)
	{
		return IsCollisionAABB(col1.aabb.min + col1Pos, col1.aabb.max + col1Pos, col2.aabb.min + col2Pos, col2.aabb.max + col2Pos);
	}
	else if (col1.type == Collider2D::Type::kCircle && col2.type == Collider2D::Type::kCircle)
	{
		return IsCollisionCircle(col1Pos, col1.circle.radius, col2Pos, col2.circle.radius);
	}
	else if (col1.type == Collider2D::Type::kLine && col2.type == Collider2D::Type::kLine)
	{
		return IsCollisionLine(col1.line.p1 + col1Pos, col1.line.p2 + col1Pos, col2.line.p1 + col2Pos, col2.line.p2 + col2Pos);
	}
	return false;
}

bool IsCollisionAABB(const D3DXVECTOR2& min1, const D3DXVECTOR2& max1, const D3DXVECTOR2& min2, const D3DXVECTOR2& max2)
{
	return min1.x < max2.x
		&& max1.x > min2.x
		&& min1.y < max2.y
		&& max1.y > min2.y;
}

bool IsCollisionAABBPos(const D3DXVECTOR2& min, const D3DXVECTOR2& max, const D3DXVECTOR2& pos)
{
	return min.x < pos.x
		&& max.x > pos.x
		&& min.y < pos.y
		&& max.y > pos.y;
}

bool IsCollisionCircle(const D3DXVECTOR2& p1, const float& r1, const D3DXVECTOR2& p2, const float& r2)
{
	auto p = p1 + p2;
	return p.x * p.x + p.y * p.y < (r1 + r2) * (r1 + r2);
}

bool IsCollisionLine(const D3DXVECTOR2& p1, const D3DXVECTOR2& p2, const D3DXVECTOR2& p3, const D3DXVECTOR2& p4)
{
	float t, s;
	float under = (p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y);
	if (under == 0.0f)
		return false;

	float _t = (p4.y - p3.y) * (p1.x - p3.x) - (p4.x - p3.x) * (p1.y - p3.y);
	float _s = (p2.y - p1.y) * (p1.x - p3.x) - (p2.x - p1.x) * (p1.y - p3.y);

	t = _t / under;
	s = _s / under;

	if (t < 0.0f || t > 1.0f || s < 0.0f || s > 1.0f)
		return false;
	if (t == 0.0f && s == 0.0f)
		return false;

	return true;
}

float D3DXVec2Angle(const D3DXVECTOR2& dest, const D3DXVECTOR2& src)
{
	auto len1 = D3DXVec2Length(&dest);
	auto len2 = D3DXVec2Length(&src);
	if (len1 == 0.0f && len2 == 0.0f)
		return 0.0f;

	float theta = acos(D3DXVec2Dot(&src, &dest) / (len1 * len2));
	
	return D3DXToDegree(theta);
}

D3DXVECTOR2 D3DXVec2Rotation(const D3DXVECTOR2& vec, const float& rotate)
{
	if (vec == D3DXVECTOR2(0.0f, 0.0f))
		return D3DXVECTOR2(0.0f, 0.0f);

	float radian = -D3DXToRadian(rotate);
	float sin = sinf(radian);
	float cos = cosf(radian);

	return D3DXVECTOR2(cos * vec.x - sin * vec.y, sin * vec.x + cos * vec.y);
}
