#include "Core.h"

PlayerInfo::PlayerInfo()
{
	hpSprite.LoadAll(L"Assets/UI/Game/Player/Hp/");
	expSprite.LoadAll(L"Assets/UI/Game/Player/Exp");
}

void PlayerInfo::Render()
{
	auto player = std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->player;

	Sprite::RenderProperty hpProperty;
	hpProperty.pos = position + D3DXVECTOR2(-240.0f, 160.0f);
	hpSprite.curScene = 0;
	hpSprite.Render(hpProperty);
	hpProperty.pos.x += 8.0f;
	hpProperty.widthRatio = (float)player->hp / (float)player->maxHp;
	hpSprite.curScene = 1;
	hpSprite.Render(hpProperty);

	Sprite::RenderProperty expProperty;
	expProperty.pos = hpProperty.pos + D3DXVECTOR2(-30.0f, -20.0f);
	expSprite.curScene = player->level;
	expSprite.Render(expProperty);
	expProperty.pos.x += 5.0f;
	expProperty.pos.y += -4.0f;
	expProperty.widthRatio = (float)player->exp / (float)player->maxExp;
	expSprite.curScene = 6;
	expSprite.Render(expProperty);
}
