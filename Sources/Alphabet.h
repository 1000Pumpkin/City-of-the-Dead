#pragma once
class Alphabet : public Object
{
public:
	Sprite apbSprite;
	std::wstring apb;

public:
	Alphabet(const std::wstring& path);

	void AddAlphabet(WCHAR alphabet);
	virtual void Render();
};

