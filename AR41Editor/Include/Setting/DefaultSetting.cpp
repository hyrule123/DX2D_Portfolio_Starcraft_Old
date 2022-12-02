#include "DefaultSetting.h"

//Resources
#include "Resource/ResourceManager.h"


#include "CDO.h"

//GameObject CDO Preload
#include "../GameObject/Player2D.h"
#include "../GameObject/Monster.h"
#include "../GameObject/Bullet.h"
#include "../GameObject/TileMapHolder.h"

#include "../UI/StartSceneUI.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "CollisionManager.h"
#include "../GameObject/Marine.h"
#include "../Component/UnitSpriteComponent.h"



CDefaultSetting::CDefaultSetting()
{
}

CDefaultSetting::~CDefaultSetting()
{
}

void CDefaultSetting::Init()
{
    LoadResource();

	CreateCDO();

	SetInput();

    SetCollision();
}

void CDefaultSetting::CreateCDO()
{
    CCDO::CreateCDO<CPlayer2D>();
    CCDO::CreateCDO<CMonster>();
    CCDO::CreateCDO<CBullet>();
    CCDO::CreateCDO<CMarine>();
    CCDO::CreateCDO<CTileMapHolder>();

    //CCDO::CreateCDO<CUnitSpriteComponent>();
    CCDO::CreateCDO<CStartSceneUI>();

    //CScene::CreateObjectCDO<CPlayer2D>("Player2D");
    //CScene::CreateObjectCDO<CMonster>("Monster");
    //CScene::CreateObjectCDO<CBullet>("Bullet");
    //CScene::CreateObjectCDO<CMarine>("Marine");

    //CScene::CreateComponentCDO<CSCUnitRootComponent>("UnitRootComponent");
    //CScene::CreateComponentCDO<CUnitSpriteComponent>("UnitSpriteComponent");

    //CScene::CreateUIWindowCDO<CStartSceneUI>("StartSceneUI");
}

void CDefaultSetting::LoadResource()
{
    std::vector<const TCHAR*>   vecFileName;

    for (int i = 1; i <= 89; ++i)
    {
        TCHAR* FileName = new TCHAR[MAX_PATH];

        memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

        wsprintf(FileName, TEXT("Explosion/Explosion%d.png"), i);

        vecFileName.push_back(FileName);
    }


    CResourceManager::GetInst()->CreateAnimationSequence2D(
        "PlayerIdle", "Explosion", vecFileName);

    CResourceManager::GetInst()->AddAnimationSequence2DFrameAll("PlayerIdle",
        89, Vector2(0.f, 0.f), Vector2(320.f, 240.f));

    for (int i = 0; i <= 88; ++i)
    {
        SAFE_DELETE_ARRAY(vecFileName[i]);
    }

    vecFileName.clear();
}

void CDefaultSetting::SetInput()
{
    // Å° µî·Ï
    CInput::GetInst()->AddBindKey("Rotation", 'D');
    CInput::GetInst()->AddBindKey("RotationInv", 'A');

    CInput::GetInst()->AddBindKey("MoveUp", 'W');
    CInput::GetInst()->AddBindKey("MoveDown", 'S');

    CInput::GetInst()->AddBindKey("Fire", VK_SPACE);

    CInput::GetInst()->AddBindKey("MoveClick", VK_RBUTTON);
}

void CDefaultSetting::SetCollision()
{
    CCollisionManager::GetInst()->CreateChannel("Player", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->CreateChannel("PlayerAttack", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->CreateChannel("Monster", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->CreateChannel("MonsterAttack", ECollision_Interaction::Collision);

    CCollisionManager::GetInst()->CreateProfile("Player", "Player", true);
    CCollisionManager::GetInst()->CreateProfile("PlayerAttack", "PlayerAttack", true);
    CCollisionManager::GetInst()->CreateProfile("Monster", "Monster", true);
    CCollisionManager::GetInst()->CreateProfile("MonsterAttack", "MonsterAttack", true);

    CCollisionManager::GetInst()->SetCollisionInteraction("Player", "PlayerAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("Player", "Player", ECollision_Interaction::Ignore);

    CCollisionManager::GetInst()->SetCollisionInteraction("PlayerAttack", "Player", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("PlayerAttack", "MonsterAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("PlayerAttack", "PlayerAttack", ECollision_Interaction::Ignore);

    CCollisionManager::GetInst()->SetCollisionInteraction("Monster", "MonsterAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("Monster", "Monster", ECollision_Interaction::Ignore);

    CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack", "Monster", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack", "MonsterAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack", "PlayerAttack", ECollision_Interaction::Ignore);
}
