#include "Core.h"

ComboUI::ComboUI()
{
	baseSprite.LoadAll(L"Assets/UI/Game/Combo.png");
	number.LoadAll(L"Assets/UI/Number", szNum);
	number.SetScale(D3DXVECTOR2(1.0f, 1.0f));
}

void ComboUI::Update(float deltaTime)
{
	auto& curComboCount = std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->player->comboCount;
	comboTime += deltaTime;

	if (curComboCount != prevComboCount)
	{
		szNum = 1;
		if (curComboCount / 10 != 0)
			++szNum;
		number.LoadAll(L"Assets/UI/Number", szNum);
		number.SetNumber(curComboCount);
		number.rp.scale = D3DXVECTOR2(1.5f, 1.5f);
		rp.scale = D3DXVECTOR2(1.5f, 1.5f);
		comboTime = 0.0f;
	}

	if (comboTime >= 1.0f)
	{
		if (curComboCount == prevComboCount && curComboCount != 0)
		{
			std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->score->AddScore(curComboCount * 1000);
			curComboCount = 0;
			number.szNum = 1;
			number.SetNumber(0);
		}
		comboTime = 0.0f;
	}

	number.Update(deltaTime * 2.0f);
	rp.scale += (destScale - rp.scale) * 20.0f * deltaTime;

	prevComboCount = curComboCount;
}

void ComboUI::Render()
{
	if (prevComboCount != 0)
	{
		rp.pos = D3DXVECTOR2(260.0f, 100.0f);
		baseSprite.Render(rp);
		number.position = rp.pos - D3DXVECTOR2(50.0f + 10.0f * (szNum - 1), 0.0f);
		number.Render();
	}
}
