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
	CCDO::CreateCDO<CSceneComponent>("SceneComponent", true);
	CCDO::CreateCDO<CSpriteComponent>("SpriteComponent", true);
	CCDO::CreateCDO<CCameraComponent>("CameraComponent", true);
	CCDO::CreateCDO<CColliderBox2D>("ColliderBox2D", true);
	CCDO::CreateCDO<CColliderOBB2D>("ColliderOBB2D", true);
	CCDO::CreateCDO<CColliderSphere2D>("ColliderSphere2D", true);
	CCDO::CreateCDO<CColliderPixel>("ColliderPixel", true);
	CCDO::CreateCDO<CTargetArm>("TargetArm", true);
	CCDO::CreateCDO<CTileMapComponent>("TileMapComponent", true);
}
