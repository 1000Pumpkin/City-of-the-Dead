#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>

#include <map>	
#include <vector>
#include <string>
#include <io.h>
#include <fstream>
#include <istream>
#include <ostream>
#include <iostream>
#include <filesystem>
#include <functional>
#include <mciapi.h>
#include <mmsystem.h>

using namespace std;

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include "Direct3D.h"
#include "Texture.h"
#include "Sprite.h"
#include "Sound.h"
#include "KeyManager.h"
#include "ObjectCollider2D.h"
#include "Object.h"
#include "Scene.h"

#include "Alphabet.h"
#include "Number.h"
#include "Button.h"
#include "Effect.h"

#include "Skill.h"
#include "SkillObject.h"
#include "SkillAttack1.h"
#include "SkillAttack2.h"
#include "SkillDash.h"
#include "SkillEnemy1.h"
#include "SkillEnemy2.h"
#include "SkillEnemy3.h"
#include "SkillBoss1.h"
#include "SkillBoss2.h"
#include "SkillBoss3.h"
#include "SkillBoss4.h"

#include "Item.h"
#include "Character.h"
#include "Player.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "Boss1.h"
#include "Boss2.h"

#include "PlayerInfo.h"
#include "PlayerSpec.h"
#include "Score.h"
#include "ComboUI.h"
#include "TileMap.h"

#include "LoadingScene.h"
#include "GameScene.h"
#include "EditorScene.h"
#include "RankScene.h"
#include "MainScene.h"