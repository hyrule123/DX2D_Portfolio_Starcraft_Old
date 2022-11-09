
#include "EditorDefaultScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneViewport.h"
#include "../UI/StartSceneUI.h"

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
	m_Owner->GetViewport()->CreateUIWindow<CStartSceneUI>("StartUI");

	return true;
}

CEditorDefaultScene* CEditorDefaultScene::Clone() const
{
	return new CEditorDefaultScene(*this);
}
