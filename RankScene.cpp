#include "Core.h"

RankScene::RankScene(int score)
{
	this->score = score;
}

void RankScene::Init()
{
	bgSprite.LoadAll(L"Assets/UI/Main/Rank.png");

	std::wstring name = L"";

	RankInfo ri;
	Load(ri);

	for (int i = 0; i < 3; ++i)
	{
		if (score > ri.scores[i])
		{
			auto _t = ri.scores[i];
			auto _n = ri.names[i];
			ri.names[i] = name;
			ri.scores[i] = score;
			score = _t;
			name = _n;
			if (overwriteScoreIDX == -1)
				overwriteScoreIDX = i;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		std::shared_ptr<Alphabet> apb(new Alphabet(L"Assets/UI/Alphabet/"));
		alphabets[i] = apb;
		apb->position = D3DXVECTOR2(-135.0f, 70.0f - 95.0f * i);
		for (auto& c : ri.names[i])
			apb->AddAlphabet(c);
		obm.AddUiObject(apb);

		std::shared_ptr<Number> num(new Number);
		numbers[i] = num;
		num->LoadAll(L"Assets/UI/Number/", 8);
		num->SetNumber(ri.scores[i]);
		num->SetScale(D3DXVECTOR2(1.0f, 1.0f));
		num->position = D3DXVECTOR2(120.0f, 70.0f - 95.0f * i);
		obm.AddUiObject(num);
	}
}

void RankScene::Update(float deltaTime)
{
	if (overwriteScoreIDX == -1 && KeyManager::GetInstance().GetKeyState(VK_LBUTTON) == KeyState::Down)
	{
		SceneManager::GetInstance().AddScene(L"Main", std::shared_ptr<Scene>(new MainScene));
		SceneManager::GetInstance().ChangeScene(L"Main");
	}

	if (overwriteScoreIDX != -1)
	{
		for (int i = L'A'; i < L'Z'; ++i)
		{
			if (KeyManager::GetInstance().GetKeyState(i) == KeyState::Down)
			{
				alphabets[overwriteScoreIDX]->AddAlphabet((WCHAR)i);
				if (alphabets[overwriteScoreIDX]->apb.size() == 3)
				{
					Save(alphabets[overwriteScoreIDX]->apb, numbers[overwriteScoreIDX]->num);
					overwriteScoreIDX = -1;
				}
			}
		}
	}

	Scene::Update(deltaTime);
}

void RankScene::Render()
{
	Scene::Render();
}

void RankScene::UiRender()
{
	Sprite::RenderProperty rp;
	bgSprite.Render(rp);

	Scene::UiRender();
}

void RankScene::Save(const std::wstring& name, const int& score)
{
	RankInfo ri;
	Load(ri);

	auto curName = name;
	auto curScore = score;
	for (int i = 0; i < 3; ++i)
	{
		if (curScore > ri.scores[i])
		{
			auto _score = curScore;
			auto _name = curName;
			curScore = ri.scores[i];
			curName = ri.names[i];
			ri.scores[i] = _score;
			ri.names[i] = _name;
		}
	}

	std::wofstream os;
	os.open(L"Assets/Ranking.txt");
	if (os.is_open())
	{
		for (int i = 0; i < 3; ++i)
		{
			os << ri.names[i] << std::endl;
			os << ri.scores[i] << std::endl;
		}
	}
	os.close();
}

void RankScene::Load(RankInfo& ri)
{
	std::wifstream is;
	is.open(L"Assets/Ranking.txt");
	if (is.is_open())
	{
		for (int i = 0; i < 3; ++i)
		{
			is >> ri.names[i];
			is >> ri.scores[i];
		}
	}
	is.close();
}
