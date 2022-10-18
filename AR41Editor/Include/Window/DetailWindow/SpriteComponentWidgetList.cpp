#include "SpriteComponentWidgetList.h"
#include "Editor/EditorTree.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorListBox.h"
#include "../Animation2DWindow.h"
#include "Editor/EditorGUIManager.h"
#include "../DetailWindow.h"
#include "Component/SpriteComponent.h"

CSpriteComponentWidgetList::CSpriteComponentWidgetList()
{
}

CSpriteComponentWidgetList::~CSpriteComponentWidgetList()
{
}

void CSpriteComponentWidgetList::SetSpriteContent(CSpriteComponent* Sprite)
{
	m_AnimationClass->SetText(Sprite->GetAnimationName().c_str());

	// 애니메이션 정보를 얻어와서 그 이름들을 리스트에 넣어준다.
	m_AnimationList->Clear();

	std::vector<std::string>	vecAnimNames;

	Sprite->GetAnimationNames(vecAnimNames);

	size_t	Size = vecAnimNames.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_AnimationList->AddItem(vecAnimNames[i]);
	}
}

bool CSpriteComponentWidgetList::Init()
{
	CEditorTree<void*>* Category = CreateWidget<CEditorTree<void*>>("SpriteComponent");

	Category->SetHideName("SpriteComponent");

	Category->SetSize(400.f, 300.f);

	Category->AddItem(nullptr, "SpriteComponent");
	Category->AddItem(nullptr, "Sprite", "SpriteComponent");
	Category->AddItem(nullptr, "Animation", "SpriteComponent");

	Category->CreateWidget<CEditorImage>("Sprite", "SpriteImage");

	Category->CreateWidget<CEditorSameLine>("Sprite", "Line");

	CEditorButton* Button = Category->CreateWidget<CEditorButton>("Sprite", "SelectTexture");

	Button->SetClickCallback<CSpriteComponentWidgetList>(this, &CSpriteComponentWidgetList::SelectTextureButtonClick);

	m_AnimationClass = Category->CreateWidget<CEditorInput>("Animation", "AnimationClass", 100.f, 30.f);
	m_AnimationClass->ReadOnly(true);
	m_AnimationClass->SetText("None");
	m_AnimationClass->SetSize(150.f, 30.f);

	Category->CreateWidget<CEditorSameLine>("Animation", "Line");

	Button = Category->CreateWidget<CEditorButton>("Animation", "CreateAnimation");

	Button->SetSize(150.f, 30.f);
	Button->SetClickCallback<CSpriteComponentWidgetList>(this, &CSpriteComponentWidgetList::CreateAnimationButtonClick);

	m_AnimationList = Category->CreateWidget<CEditorListBox>("Animation", "AnimationList");

	m_AnimationList->SetHideName("AnimationList");

	m_AnimationList->SetSize(150.f, 300.f);
	m_AnimationList->SetPageItemCount(6);
	m_AnimationList->SetSelectCallback<CSpriteComponentWidgetList>(this, &CSpriteComponentWidgetList::AnimationSelectCallback);

	Category->CreateWidget<CEditorSameLine>("Animation", "Line");

	CEditorGroup* AnimDataGroup = Category->CreateWidget<CEditorGroup>("Animation", "Animation2DData");

	m_PlayTime = AnimDataGroup->CreateWidget<CEditorInput>("PlayTime", 100.f, 30.f);
	m_PlayTime->SetInputType(EImGuiInputType::Float);

	m_PlayScale = AnimDataGroup->CreateWidget<CEditorInput>("PlayScale", 100.f, 30.f);
	m_PlayScale->SetInputType(EImGuiInputType::Float);

    return true;
}

void CSpriteComponentWidgetList::SelectTextureButtonClick()
{
}

void CSpriteComponentWidgetList::CreateAnimationButtonClick()
{
	CAnimation2DWindow* Animation2DWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CAnimation2DWindow>("Animation2DWindow");

	std::string Name = Animation2DWindow->GetAnimation2DSelectName();

	if (Name.empty())
		return;

	CSpriteComponent* Sprite = (CSpriteComponent*)m_DetailWindow->GetSelectComponent();

	if (Name == "Animation2D")
	{
		Sprite->SetAnimation<CAnimation2D>("Animation2D");
	}
}

void CSpriteComponentWidgetList::AnimationSelectCallback(int Index, const std::string& Item)
{
}
