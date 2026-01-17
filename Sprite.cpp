#include "Core.h"

void Sprite::AniInit()
{
	curScene = 0;
	aniTime = 0.0f;
}

void Sprite::LoadAll(std::wstring path)
{
	textures.clear();

	filePath = path;

	if (std::filesystem::is_directory(filePath))
	{
		for (auto& file : std::filesystem::recursive_directory_iterator(filePath))
		{
			if (auto texture = TextureManager::GetInstance().GetTexture(file.path()))
				textures.push_back(texture);
		}
	}
	else
	{
		if (auto texture = TextureManager::GetInstance().GetTexture(filePath))
			textures.push_back(texture);
	}

	maxScene = textures.size();
}

bool Sprite::Update(float deltaTime)
{
	if(deltaTime < 1.0f / 30.0f)
		aniTime += deltaTime;
	if (aniTime >= aniMaxTime)
	{
		aniTime -= aniMaxTime;
		if (++curScene >= maxScene)
		{
			if (isLoop)
				curScene = 0;
			else
				curScene = maxScene - 1;
			return true;
		}
	}
	return false;
}

void Sprite::Render(const RenderProperty& rp)
{
	std::shared_ptr<Texture> curTexture;

	if (curScene < 0 || curScene >= maxScene)
	{
		curTexture = TextureManager::GetInstance().GetTexture(L"Assets/not_found.png");
	}
	else
	{
		curTexture = textures[curScene];
	}

	D3DXVECTOR2 texSize(curTexture->info.Width, curTexture->info.Height);
	auto color = D3DCOLOR_ARGB((int)(rp.a * 255.0f), (int)(rp.r * 255.0f), (int)(rp.g * 255.0f), (int)(rp.b * 255.0f));

	auto& D3D = Direct3D::GetInstance();

	CUSTOMVERTEX* pVertices;
	if (FAILED(D3D.pVB->Lock(0, 0, (void**)&pVertices, 0)))
		return;

	pVertices[0].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVertices[0].color = color;
	pVertices[0].uv = D3DXVECTOR2(0.0f, 1.0f);

	pVertices[1].position = D3DXVECTOR3(0.0f, texSize.y, 0.0f);
	pVertices[1].color = color;
	pVertices[1].uv = D3DXVECTOR2(0.0f, 0.0f);
	
	pVertices[2].position = D3DXVECTOR3(texSize.x * rp.widthRatio, 0.0f, 0.0f);
	pVertices[2].color = color;
	pVertices[2].uv = D3DXVECTOR2(rp.widthRatio, 1.0f);

	pVertices[3].position = D3DXVECTOR3(texSize.x * rp.widthRatio, texSize.y, 0.0f);
	pVertices[3].color = color;
	pVertices[3].uv = D3DXVECTOR2(rp.widthRatio, 0.0f);

	D3D.pVB->Unlock();

	D3DXMATRIX matWorld;
	D3DXVECTOR2 center(rp.center.x * texSize.x, rp.center.y * texSize.y);
	D3DXVECTOR2 scaleCen(rp.scaleCen.x * texSize.x + center.x, rp.scaleCen.y * texSize.y + center.y);
	D3DXVECTOR2 rotateCen(rp.rotateCen.x * texSize.x + center.x, rp.rotateCen.y * texSize.y + center.y);
	D3DXVECTOR2 pos(rp.pos - center);

	D3DXMatrixTransformation2D(&matWorld, &scaleCen, 0.0f, &rp.scale, &rotateCen, -D3DXToRadian(rp.rotate), &pos);
	D3D.pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	D3D.pd3dDevice->SetTexture(0, curTexture->src);
	D3D.pd3dDevice->SetStreamSource(0, D3D.pVB, 0, sizeof(CUSTOMVERTEX));
	D3D.pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	D3D.pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
