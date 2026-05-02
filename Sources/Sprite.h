#pragma once

class Sprite
{
public:
	class RenderProperty
	{
	public:
		D3DXVECTOR2 pos = D3DXVECTOR2(0.0f, 0.0f);
		D3DXVECTOR2 scale = D3DXVECTOR2(1.0f, 1.0f);
		D3DXVECTOR2 scaleCen = D3DXVECTOR2(0.0f, 0.0f);
		D3DXVECTOR2 rotateCen = D3DXVECTOR2(0.0f, 0.0f);
		D3DXVECTOR2 center = D3DXVECTOR2(0.5f, 0.5f);

		float rotate = 0.0f;
		float a = 1.0f, r = 1.0f, g = 1.0f, b = 1.0f;
		float widthRatio = 1.0f;
	};

	std::vector<std::shared_ptr<Texture>> textures;
	std::wstring filePath;

	int curScene = 0, maxScene = 0;
	float aniTime = 0.0f, aniMaxTime = 0.1f;
	bool isLoop = true;

public:
	void AniInit();
	void LoadAll(std::wstring path);
	bool Update(float deltaTime);
	void Render(const RenderProperty& rp);
};