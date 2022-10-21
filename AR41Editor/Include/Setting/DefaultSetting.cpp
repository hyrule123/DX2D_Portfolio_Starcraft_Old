#include "DefaultSetting.h"
#include "../GameObject/Player2D.h"
#include "../GameObject/Monster.h"
#include "../GameObject/Bullet.h"
#include "../UI/StartSceneUI.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "CollisionManager.h"
#include "../GameObject/Marine.h"
#include "../Component/UnitRootComponent.h"
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
    CScene::CreateObjectCDO<CPlayer2D>("Player2D");
    CScene::CreateObjectCDO<CMonster>("Monster");
    CScene::CreateObjectCDO<CBullet>("Bullet");
    CScene::CreateObjectCDO<CMarine>("Marine");

    CScene::CreateComponentCDO<CUnitRootComponent>("UnitRootComponent");
    CScene::CreateComponentCDO<CUnitSpriteComponent>("UnitSpriteComponent");

    CScene::CreateUIWindowCDO<CStartSceneUI>("StartSceneUI");
}

void CDefaultSetting::LoadResource()
{
    CResourceManager::GetInst()->CreateAnimationSequence2D(
        "UltraIdle", "UltraLisk", TEXT("_SCAssets\\Unit\\zerg\\ultra(Ultralisk).bmp"));

    CTexture* UltraTex = CResourceManager::GetInst()->FindTexture("UltraLisk");


    if (!UltraTex)
        assert(0);
    CResourceManager::GetInst()->AddAnimationSequence2DFrameByTileNumber("UltraIdle", EAnimation2DType::Array, 17, 16, 0, 1);

    CResourceManager::GetInst()->CreateAnimationSequence2D(
        "UltraMove", UltraTex);
    CResourceManager::GetInst()->AddAnimationSequence2DFrameByTileNumber("UltraMove", EAnimation2DType::Array, 17, 16, 1, 9);


    CResourceManager::GetInst()->CreateAnimationSequence2D(
        "UltraAttack", UltraTex);
    CResourceManager::GetInst()->AddAnimationSequence2DFrameByTileNumber("UltraAttack", EAnimation2DType::Array, 17, 16, 10, 5);

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
