#pragma once
class Button : public Object
{
public:
	enum State
	{
		Click,
		Disable,
		Hover,
		None
	};

	State state = State::None;
	Sprite btnSprite;
	bool eventHandler = true;
	std::function<void()> OnClick;
	std::function<void()> OnHover;

	virtual void Update(float deltaTime);
	virtual void Render();
};

