#include "Core.h"

Alphabet::Alphabet(const std::wstring& path)
{
	apbSprite.LoadAll(path);
}

void Alphabet::AddAlphabet(WCHAR alphabet)
{
	if (alphabet < L'A' || alphabet > L'Z')
		return;

	apb += alphabet;
}

void Alphabet::Render()
{
	Sprite::RenderProperty rp;
	float offset = 0.0f;
	for (auto& c : apb)
	{
		rp.pos = position + D3DXVECTOR2(offset, 0.0f);
		apbSprite.curScene = c - L'A';
		apbSprite.Render(rp);
		offset += apbSprite.textures[apbSprite.curScene]->info.Width;
	}
}
