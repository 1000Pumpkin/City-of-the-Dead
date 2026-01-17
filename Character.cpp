#include "Core.h"

std::shared_ptr<Sound> Character::hitSound(new Sound(L"Assets/SoundFX/Attack.mp3", 10));
std::shared_ptr<Sound> Character::hit2Sound(new Sound(L"Assets/SoundFX/AttackP.mp3", 10));

bool Character::CmdCast(Skill* skill)
{
	if (curSkill)
	{
		if (curSkill == skill)
		{
			if (skill->Casting())
				return true;
		}
	}

	if (bodyState != State::kDead && bodyState != State::kHit && bodyState != State::kSkill)
	{
		if (skill->IsCoolTime())
		{
			if (tag == L"Player")
			{
				std::shared_ptr<Effect> eff(new Effect(L"Assets/UI/Game/Message/"));
				eff->position = position + D3DXVECTOR2(0.0f, 50.0f);
				eff->duration = 0.2f;
				SceneManager::GetInstance().curScene->obm.AddObject(eff);
			}
		}
		if (skill->Casting())
		{
			curSkill = skill;
			return true;
		}
	}
	return false;
}

void Character::OnHit(int damage, D3DXVECTOR2 elasticForce, float duration, std::wstring skillName)
{
	hp -= damage;
	this->elasticForce += elasticForce * elasticScale;
	hitDuration = duration * hitDurationScale;

	if (tag != L"Player")
	{
		std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->score->AddScore(damage * 10);
		if (skillName == L"Attack")
		{
			hitSound->Play(false);
		}
		if (skillName == L"AttackP")
		{
			hit2Sound->Play(false);
		}

	}

	std::shared_ptr<Effect> hitEff(new Effect(L"Assets/CommonFX/HitCircleFX/"));
	hitEff->position = D3DXVECTOR2(0.0f, 30.f);
	hitEff->rp.scale *= 0.5f;
	hitEff->object = this;
	SceneManager::GetInstance().curScene->obm.AddObject(hitEff);

	rp.g *= 0.2f;
	rp.b *= 0.2f;

	std::shared_ptr<Effect> numEff(new Effect(L"Assets/UI/Number/", damage, D3DXVECTOR2(0.5f, 0.5f), D3DXVECTOR2(0.8f, 0.8f)));
	numEff->position = D3DXVECTOR2(0.0f, 50.0f);
	numEff->duration = 0.5f;
	numEff->object = this;
	SceneManager::GetInstance().curScene->obm.AddObject(numEff);

	if (curSkill)
	{
		if (curSkill->Cancel())
			curSkill = NULL;
	}
}

Character::Character(std::wstring name) : name(name)
{
	WCHAR buffer[255];
	for (int i = 0; i < State::kSkill; ++i)
	{
		if (i == State::kJump || i == State::kDead)
		{
			bodySprite[i].isLoop = false;
			armSprite[i].isLoop = false;
		}

		swprintf_s(buffer, L"Assets/Character/%s/Body/%s/", name.c_str(), stateStrs[i]);
		bodySprite[i].LoadAll(buffer);

		swprintf_s(buffer, L"Assets/Character/%s/Arms/%s/", name.c_str(), stateStrs[i]);
		armSprite[i].LoadAll(buffer);
	}
	hpSprite.LoadAll(L"Assets/UI/Game/Enemy/Hp/");

	rp.center.y = 0.25f;
}

Character::~Character()
{
	for (auto skill : skills)
	{
		skill->Cancel(true);
		delete skill;
	}
	skills.clear();
}

void Character::Update(float deltaTime)
{
	if (bodyState != State::kSkill)
		bodySprite[bodyState].Update(deltaTime);
	if (armState != State::kSkill)
		armSprite[armState].Update(deltaTime);
}

void Character::LateUpdate(float deltaTime)
{
}

void Character::FixedUpdate(float fixedDeltaTime)
{
	if (exp >= maxExp)
	{
		if (level < 5)
		{
			++level;
			++sp;
			exp = 0;
		}
		else
		{
			exp = maxExp;
		}
	}

	rp.g += fixedDeltaTime * 2.0f;
	rp.b += fixedDeltaTime * 2.0f;
	if (rp.g >= 1.0f)
		rp.g = 1.0f;
	if (rp.b >= 1.0f)
		rp.b = 1.0f;

	for (auto& skill : skills)
		skill->FixedUpdate(fixedDeltaTime);

	bool isCanJump = true;
	if (bodyState == State::kHit || bodyState == State::kDead || (curSkill && !curSkill->casterMove))
		isCanJump = false;

	if (isCanJump && curJumpCount < nextJumpCount && curJumpCount < 2)
	{
		velocity.y = 7.0f;
		++curJumpCount;

		bodySprite[State::kJump].AniInit();
		armSprite[State::kJump].AniInit();
	}

	if (hp <= 0)
	{
		hp = 0;
		bodyNextState = State::kDead;
		armNextState = State::kDead;
		isActiveCollider = false;

		if (bodyState != State::kDead)
		{
			if (tag == L"Enemy")
			{
				std::static_pointer_cast<GameScene>(SceneManager::GetInstance().curScene)->player->exp += 5;
			}
			if (name != L"Player" && name != L"EnemyBoss1" && name != L"EnemyBoss2")
			{
				int random = rand() % 5;
				if (random == 0)
					SceneManager::GetInstance().curScene->obm.AddObject(std::shared_ptr<Object>(new Item(rand() % 2,
						position + D3DXVECTOR2(0.0f, 50.0f))));
			}
		}

		if (curSkill)
		{
			curSkill->Cancel(true);
			curSkill = NULL;
		}
	}
	else if (hitDuration > 0.0f)
	{
		hitDuration -= fixedDeltaTime;
		bodyNextState = State::kHit;
		armNextState = State::kHit;
	}
	else if (isGravity && velocity.y > 0.0f)
	{
		bodyNextState = State::kJump;
		armNextState = State::kJump;
	}
	else if (isGravity && velocity.y < 0.0f)
	{
		bodyNextState = State::kJumpDropped;
		armNextState = State::kJumpDropped;
	}
	else if (moveDir)
	{
		bodyNextState = State::kMove;
		armNextState = State::kMove;
	}
	else
	{
		if (bodyState == State::kMove)
		{
			bodyNextState = State::kMoveStop;
			armNextState = State::kMoveStop;
			moveStopAniTime = moveStopAniMaxTime;
		}
		else if (bodyState == State::kMoveStop)
		{
			moveStopAniTime -= fixedDeltaTime;
			if (moveStopAniTime <= 0.0f)
			{
				bodyNextState = State::kStay;
				armNextState = State::kStay;
			}
		}
		else
		{
			bodyNextState = State::kStay;
			armNextState = State::kStay;
		}
	}

	if (curSkill)
	{
		if (curSkill->IsSkillDone())
		{
			if (!curSkill->casterMove)
				bodyNextState = State::kStay;
			armNextState = State::kStay;
			curSkill = NULL;
		}
		else
		{
			if (!curSkill->casterMove)
				bodyNextState = State::kSkill;
			armNextState = State::kSkill;
		}
	}

	if (bodyState != bodyNextState)
	{
		if (bodyState != State::kSkill)
			bodySprite[bodyState].AniInit();
		bodyState = bodyNextState;
	}
	if (armState != armNextState)
	{
		if (armState != State::kSkill)
			armSprite[armState].AniInit();
		armState = armNextState;
	}

	if (curSkill)
		curSkill->SpriteUpdate(fixedDeltaTime);

	linear = D3DXVECTOR2(0.0f, 0.0f);
	if (bodyState == State::kMove || bodyState == State::kJump || bodyState == State::kJumpDropped)
	{
		linear.x = moveDir * moveSpeed * fixedDeltaTime;
	}
	else if (bodyState == State::kMoveStop)
	{
		linear.x = rp.scale.x * moveStopSlip * fixedDeltaTime;
	}
	elasticForce *= 0.9f;
	if (D3DXVec2Length(&elasticForce) < 0.1f)
		elasticForce = D3DXVECTOR2(0.0f, 0.0f);

	if (isGravity)
		velocity.y -= forceGravity * fixedDeltaTime;

	D3DXVECTOR2 move = linear + velocity + elasticForce;

	auto nextTile = TileMapManager::GetInstance().GetTile(0, TileMapManager::GetInstance().WP2TP(position + D3DXVECTOR2(0.0f, move.y)));
	if (nextTile)
	{
		if (nextTile->type == Tile::Type::Ground && isColliderGroundTile)
		{
			if (move.y < 0.0f)
			{
				position.y = TileMapManager::GetInstance().TP2WP(nextTile->position).y + TILEHEIGHT * 0.5f + (forceGravity * fixedDeltaTime * 0.5f);
				move.y = velocity.y = elasticForce.y = 0;
				curJumpCount = nextJumpCount = 0;
			}
		}
		else if (nextTile->type == Tile::Type::Wall)
		{
			if (move.y < 0.0f)
			{
				position.y = TileMapManager::GetInstance().TP2WP(nextTile->position).y + TILEHEIGHT * 0.5f + 0.1f;
				move.y = velocity.y = elasticForce.y = 0;
				curJumpCount = nextJumpCount = 0;
			}
			else if (move.y > 0.0f)
			{
				position.y = TileMapManager::GetInstance().TP2WP(nextTile->position).y - TILEHEIGHT * 0.5f - 0.1f;
				move.y = velocity.y = elasticForce.y = 0;
			}
		}
	}
	nextTile = TileMapManager::GetInstance().GetTile(0, TileMapManager::GetInstance().WP2TP(position + D3DXVECTOR2(move.x, 0.0f)));
	if (nextTile)
	{
		if (nextTile->type == Tile::Type::Wall)
		{
			if (move.x < 0.0f)
			{
				position.x = TileMapManager::GetInstance().TP2WP(nextTile->position).x + TILEWIDTH * 0.5f + 0.1f;
				move.x = velocity.x = elasticForce.x = 0;
			}
			else if (move.x > 0.0f)
			{
				position.x = TileMapManager::GetInstance().TP2WP(nextTile->position).x - TILEWIDTH * 0.5f - 0.1f;
				move.x = velocity.x = elasticForce.x = 0;
			}
		}
	}

	if (move.x > 0.0f)
	{
		rp.scale.x = bodyState == State::kHit ? -1.0f : 1.0f;
	}
	else if (move.x < 0.0f)
	{
		rp.scale.x = bodyState == State::kHit ? 1.0f : -1.0f;
	}

	position = position + move;
}

void Character::Render()
{
	rp.pos = position;
	
	if (!curSkill)
	{
		if (armSprite[armState].maxScene != 0)
			armSprite[armState].Render(rp);
		bodySprite[bodyState].Render(rp);
	}

	if (curSkill && curSkill->casterMove)
	{
		bodySprite[bodyState].Render(rp);
	}

	if (curSkill)
	{
		curSkill->SpriteRender(rp);
	}

	if (tag == L"Enemy")
	{
		Sprite::RenderProperty _rp = rp;
		_rp.scale = hpScale;
		_rp.pos += hpOffset;
		hpSprite.curScene = 0;
		hpSprite.Render(_rp);
		_rp.widthRatio = (float)hp / (float)maxHp;
		hpSprite.curScene = 1;
		hpSprite.Render(_rp);
	}
	Object::Render();
}

void Character::OnCollision(Collider2D& collision)
{
}
