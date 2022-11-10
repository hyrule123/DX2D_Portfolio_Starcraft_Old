#include "EngineSetting.h"
#include "Scene/SceneInfo.h"
#include "GameObject/GameObject.h"

//Components
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderOBB2D.h"
#include "Component/ColliderPixel.h"
#include "Component/ColliderSphere2D.h"

#include "Component/TargetArm.h"
#include "Component/Tile.h"
#include "Component/TileMapComponent.h"

CEngineSetting::CEngineSetting()
{
}

CEngineSetting::~CEngineSetting()
{
}

void CEngineSetting::Init()
{
	CCDO::CreateCDO<CSceneComponent>();
	CCDO::CreateCDO<CSpriteComponent>();
	CCDO::CreateCDO<CCameraComponent>();
	CCDO::CreateCDO<CColliderBox2D>();
	CCDO::CreateCDO<CColliderOBB2D>();
	CCDO::CreateCDO<CColliderSphere2D>();
	CCDO::CreateCDO<CColliderPixel>();
	CCDO::CreateCDO<CTargetArm>();
	CCDO::CreateCDO<CTileMapComponent>();
}
