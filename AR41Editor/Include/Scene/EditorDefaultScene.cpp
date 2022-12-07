
#include "EditorDefaultScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneViewport.h"
#include "../UI/StartSceneUI.h"

//Used Objects
#include "../GameObject/TileMapHolder.h"
#include "../GameObject/Marine.h"
#include "../GameObject/SCUnitInstance.h"

CEditorDefaultScene::CEditorDefaultScene()
{
	m_ClassTypeName = "EditorDefaultScene";
}

CEditorDefaultScene::CEditorDefaultScene(const CEditorDefaultScene& Scene):
	CSceneInfo(Scene)
{
}

CEditorDefaultScene::~CEditorDefaultScene()
{
}

bool CEditorDefaultScene::Init()
{
	//m_Owner->GetViewport()->CreateUIWindow<CStartSceneUI>("StartUI");

	m_Owner->CreateObject<CTileMapHolder>("TileMapHolder");
	m_Owner->CreateObject<CSCUnitInstance>("SCUnitInstance");
	//m_Owner->CreateObject<CMarine>("Marine");

	return true;
}

CEditorDefaultScene* CEditorDefaultScene::Clone() const
{
	return new CEditorDefaultScene(*this);
}
