#include "Core.h"

void Skill::LevelUp()
{
	if (++level > maxLevel)
		level = maxLevel;

	OnLevelChanged(level);
}

bool Skill::Casting()
{
	if (IsCasting())
		return false;

	nextState = State::kCast;
	if (curState == State::kReady && chargeTime[curPhase] != 0.0f)
	{
		nextState = State::kCharge;
	}
	return true;
}

bool Skill::Cancel(bool force)
{
	if (!force && !isCancel)
		return false;

	nextState = State::kCancel;
	return true;
}

Skill::Skill(Character* caster)
{
	this->caster = caster;
}

void Skill::FixedUpdate(float fixedDeltaTime)
{
	if (curState != nextState)
	{
		if (curState == State::kPhase && nextState == State::kCast)
			++curPhase;

		if (nextState == State::kCharge)
			OnCharge();
		else if (nextState == State::kCast)
		{
			float ratio = 1.0f;
			if (curState == State::kCharge)
			{
				if (time > chargeTime[curPhase])
					time = chargeTime[curPhase];
				ratio = time / chargeTime[curPhase];
			}
			OnCast(ratio);
		}
		else if (nextState == State::kCancel)
			OnCancel();
		else if (curState == State::kCast && nextState == State::kCool)
			OnExecute();
		else if (nextState == State::kReady)
			curPhase = 0;
		time = 0.0f;
		curState = nextState;
	}

	switch (curState)
	{
	case Skill::kCharge:
		if (time > chargeTime[curPhase])
			nextState = State::kCast;
		break;
	case Skill::kCast:
		if (time > castingTime[curPhase])
		{
			if (maxPhase > curPhase)
				nextState = State::kPhase;
			else
				nextState = State::kCool;
		}
		break;
	case Skill::kPhase:
		if (time > phaseTime[curPhase])
			nextState = State::kCancel;
		break;
	case Skill::kCancel:
		nextState = State::kCool;
		break;
	case Skill::kCool:
		if (time > coolTime[curPhase])
			nextState = State::kReady;
		break;
	}

	OnUpdate(fixedDeltaTime);

	time += fixedDeltaTime;
}

bool Skill::IsSkillDone()
{
	return curState != State::kCool && nextState == State::kCool;
}

bool Skill::IsCasting()
{
	return curState != State::kReady && curState != State::kCharge && curState != State::kPhase;
}

bool Skill::IsCoolTime()
{
	return curState == State::kCool;
}

bool Skill::IsOnceTime(State state, float t, float fixedDeltaTime)
{
	return curState == state && time <= t && time + fixedDeltaTime > t;
}
