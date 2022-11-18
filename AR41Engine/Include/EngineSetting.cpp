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
	//CSV 파싱용 리소스 타입 등록
	CCDO::AddResourceType(MESH, EResourceType::Mesh);
	CCDO::AddResourceType(SHADER, EResourceType::Shader);
	CCDO::AddResourceType(CBUFFER, EResourceType::CBuffer);
	CCDO::AddResourceType(TEXTURE, EResourceType::Texture);
	CCDO::AddResourceType(MATERIAL, EResourceType::Material);
	CCDO::AddResourceType(ANIMATION, EResourceType::Animation);
	CCDO::AddResourceType(SOUND, EResourceType::Sound);
	CCDO::AddResourceType(FONT, EResourceType::Font);
	CCDO::AddResourceType(FONTCOLLECTION, EResourceType::FontCollection);
	CCDO::AddResourceType(MAP, EResourceType::Map);
	CCDO::AddResourceType(RESOURCEEND, EResourceType::End);

	CCDO::CreateCDO<CGameObject>();
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
