#include "Core.h"

void PlayerSpec::Learn(Type type, std::shared_ptr<Player> p)
{
	isLearnType[type] = true;
	switch (type)
	{
	case PlayerSpec::Attack2:
		p->skills[0]->LevelUp();
		break;
	case PlayerSpec::AttackP2:
		p->skills[1]->LevelUp();
		break;
	case PlayerSpec::SwordAura:
		p->skills[0]->LevelUp();
		p->skills[1]->LevelUp();
		break;
	case PlayerSpec::Avoid:
		p->moveSpeed = p->moveSpeed * 1.2f;
		p->damageRate = 1.2f;
		break;
	case PlayerSpec::Avoid2:
		p->skills[2]->LevelUp();
		break;
	}
}

bool PlayerSpec::IsCanLearn(Type type)
{
	switch (type)
	{
	case PlayerSpec::SwordAura:
		return isLearnType[Attack2] && isLearnType[AttackP2] && isLearnType[Avoid2];
		break;
	case PlayerSpec::Avoid2:
		return isLearnType[Avoid];
		break;
	}
	return true;
}

PlayerSpec::PlayerSpec()
{
	for (int i = 0; i < PlayerSpec::Count; ++i)
	{
		WCHAR buffer[255];
		swprintf_s(buffer, L"Assets/UI/Game/PlayerSpecfic/Buttons/%s/", typeStr[i]);
		typeBtn[i].btnSprite.LoadAll(buffer);
		typeBtn[i].OnClick = [&, i]()
		{
			if (isLearnType[i])
				return;

			if (!IsCanLearn((Type)i))
				return;

			auto player = std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->player;
			if (player->sp <= 0)
				return;

			Learn((Type)i, player);

			--player->sp;
		};

		typeBtn[i].OnHover = [&, i]()
		{
			curDescIDX = i;
		};
	}

	bgSprite.LoadAll(L"Assets/UI/Game/PlayerSpecfic/Specfic.png");
	descSprite.LoadAll(L"Assets/UI/Game/PlayerSpecfic/Desc/");
	spNumber.LoadAll(L"Assets/UI/Number/", 1);
	spNumber.position = D3DXVECTOR2(75.0f, 105.0f);
	spNumber.SetScale(D3DXVECTOR2(1.3f, 1.3f));

	typeBtn[Attack].position = D3DXVECTOR2(-180.0f, -120.0f);
	typeBtn[AttackP].position = D3DXVECTOR2(-60.0f, -120.0f);
	typeBtn[Avoid].position = D3DXVECTOR2(60.0f, -120.0f);
	typeBtn[Attack2].position = D3DXVECTOR2(-180.0f, 20.0f);
	typeBtn[AttackP2].position = D3DXVECTOR2(-60.0f, 20.0f);
	typeBtn[Avoid2].position = D3DXVECTOR2(60.0f, 20.0f);
	typeBtn[SwordAura].position = D3DXVECTOR2(-60.0f, 60.0f);
}

void PlayerSpec::Update(float deltaTime)
{
	for (int i = 0; i < Type::Count; ++i)
	{
		typeBtn[i].eventHandler = isDisplay;
		typeBtn[i].Update(deltaTime);
		if (isLearnType[i])
			typeBtn[i].btnSprite.curScene = Button::State::Click;
	}

	auto player = std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->player;
	spNumber.SetNumber(player->sp);
	spNumber.Update(deltaTime);
}

void PlayerSpec::Render()
{
	if (!isDisplay)
		return;

	Sprite::RenderProperty rp;
	bgSprite.Render(rp);
	for (int i = 0; i < Type::Count; ++i)
	{
		typeBtn[i].Render();
	}

	if (curDescIDX != -1)
	{
		Sprite::RenderProperty descProperty;
		descProperty.pos = D3DXVECTOR2(175.0f, -40.0f);
		descProperty.scale = D3DXVECTOR2(0.9f, 0.9f);
		descSprite.curScene = curDescIDX;
		descSprite.Render(descProperty);
	}

	spNumber.Render();
}
