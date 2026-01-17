#include "Core.h"

std::shared_ptr<Texture> TextureManager::GetTexture(std::wstring path)
{
	auto tf = texMap.find(path);
	if (tf != texMap.end())
		return tf->second;

	std::shared_ptr<Texture> texture(new Texture);

	auto& D3D = Direct3D::GetInstance();
	if (FAILED(D3DXCreateTextureFromFileEx(
		D3D.pd3dDevice,
		path.c_str(),
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
		0, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT,
		0,
		&texture->info,
		NULL,
		&texture->src
	)))
		return NULL;

	texMap.insert(std::make_pair(path, texture));

	return texture;
}
