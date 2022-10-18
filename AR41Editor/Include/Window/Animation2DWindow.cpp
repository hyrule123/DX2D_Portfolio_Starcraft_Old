#include "Animation2DWindow.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorText.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorTree.h"
#include "Editor/EditorGroup.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "../GameObject\Player2D.h"
#include "../GameObject\Bullet.h"
#include "../GameObject\Monster.h"
#include "ObjectWindow.h"
#include "ComponentWindow.h"
#include "Editor/EditorGUIManager.h"
#include "ResourceWindow.h"
#include "Engine.h"
#include "Resource/ResourceManager.h"
#include "Resource/Animation/AnimationSequence2D.h"

#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"

CAnimation2DWindow::CAnimation2DWindow()
{
}

CAnimation2DWindow::~CAnimation2DWindow()
{
}

bool CAnimation2DWindow::Init()
{
	CEditorLabel* Label = CreateWidget<CEditorLabel>("Animation2DClass");

	Label->SetColor(255, 0, 0, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSize(150.f, 30.f);

	CEditorSameLine* Line = CreateWidget<CEditorSameLine>("Line");

	CEditorButton* Anim2DCreateButton = CreateWidget<CEditorButton>("Anim2DCreate");
	Anim2DCreateButton->SetSize(140.f, 30.f);
	Anim2DCreateButton->SetClickCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::CreateAnimation2D);


	//Line = CreateWidget<CEditorSameLine>("Line");
	//Line->SetOffsetX(330.f);

	m_Animation2DClassList = CreateWidget<CEditorListBox>("Animation2DClassListBox");

	m_Animation2DClassList->SetHideName("Animation2DClassListBox");

	m_Animation2DClassList->SetSize(300.f, 300.f);
	m_Animation2DClassList->SetPageItemCount(6);
	m_Animation2DClassList->SetSelectCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::Animation2DSelectCallback);

	//Line = CreateWidget<CEditorSameLine>("Line");
	//Line->SetOffsetX(330.f);

	Label = CreateWidget<CEditorLabel>("Sequence2D");

	Label->SetColor(255, 0, 0, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSize(150.f, 30.f);


	Line = CreateWidget<CEditorSameLine>("Line");

	CEditorGroup* Anim2DSequenceGroup = CreateWidget<CEditorGroup>("Anim2DGroup");

	Label = Anim2DSequenceGroup->CreateWidget<CEditorLabel>("Texture");

	Label->SetColor(0, 0, 255, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSize(150.f, 30.f);




	LoadAnimation2DClassList();


    return true;
}

void CAnimation2DWindow::Update(float DeltaTime)
{
	CEditorWindow::Update(DeltaTime);
}

void CAnimation2DWindow::LoadAnimation2DClassList()
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(ROOT_PATH);

	char	Path[MAX_PATH] = {};

	strcpy_s(Path, Info->PathMultibyte);

	int	Length = (int)strlen(Path);

	// Root Path에서 Bin\을 지워준다.
	for (int i = Length - 2; i >= 0; --i)
	{
		if (Path[i] == '/' || Path[i] == '\\')
		{
			memset(&Path[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
			break;
		}
	}

	char	Directory[MAX_PATH] = {};

	strcpy_s(Directory, Path);

	// Editor의 GameObject폴더에 있는 파일을 읽어온다.
	strcat_s(Directory, "Include/Animation2D/");

	for (const auto& file : std::filesystem::recursive_directory_iterator(Directory))
	{
		char	Name[256] = {};
		char	FullPath[MAX_PATH] = {};
		char	Ext[_MAX_EXT] = {};
		char	FolderName[12] = {};

		strcpy_s(FullPath, file.path().generic_string().c_str());

		int	Length = (int)strlen(FullPath);

		if (Length >= 11)
		{
			for (int i = Length - 12; i >= 0; --i)
			{
				memcpy(FolderName, &FullPath[i], 11);

				if (strcmp(FolderName, "/Animation/") == 0)
				{
					strcpy_s(Name, &FullPath[i + 11]);
					break;
				}
			}
		}

		_splitpath_s(FullPath, nullptr, 0, nullptr, 0, nullptr, 0, Ext, _MAX_EXT);

		if (strcmp(Ext, ".cpp") == 0)
			continue;

		Length = (int)strlen(Name);

		Name[Length - 1] = 0;
		Name[Length - 2] = 0;

		m_Animation2DClassList->AddItem(Name);
	}

	Length = (int)strlen(Path);

	// Root Path에서 Bin\을 지워준다.
	for (int i = Length - 2; i >= 0; --i)
	{
		if (Path[i] == '/' || Path[i] == '\\')
		{
			memset(&Path[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
			break;
		}
	}

	m_Animation2DClassList->AddItem("Animation2D");
}


void CAnimation2DWindow::CreateAnimation2D()
{
}




void CAnimation2DWindow::Animation2DSelectCallback(int Index, const std::string& Item)
{
}

