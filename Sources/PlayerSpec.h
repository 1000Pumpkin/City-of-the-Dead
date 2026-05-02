#pragma once
class PlayerSpec : public Object
{
public:
	enum Type
	{
		Attack,
		Attack2,
		AttackP,
		AttackP2,
		SwordAura,
		Avoid,
		Avoid2,
		Count
	};

	static constexpr WCHAR* typeStr[Type::Count] = {
		L"Attack",
		L"Attack+",
		L"AttackP",
		L"AttackP+",
		L"Awaken",
		L"BodyAwaken",
		L"Evade"
	};

	bool isLearnType[Type::Count] = {
		true,
		false,
		true,
		false,
		false,
		false,
		false
	};

	void Learn(Type type, std::shared_ptr<Player> p);
	bool IsCanLearn(Type type);

	Sprite bgSprite;
	Sprite descSprite;
	int curDescIDX = -1;
	Button typeBtn[Type::Count];
	Number spNumber;
	bool isDisplay = false;

public:
	PlayerSpec();

	virtual void Update(float deltaTime);
	virtual void Render();
};

