#pragma once
class LoadingScene : public Scene
{
public:
	Sprite ldSprite;
	Sprite::RenderProperty ldProperty;
	vector<string> assetFiles;
	int curIndex = 0;

	virtual void Init() override;
	virtual void Term() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

	vector<string> GetAssetFiles(string path);
};

