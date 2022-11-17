
#include "Scene.h"
#include "../GameObject/GameObject.h"
#include "../Input.h"
#include "../Component/SpriteComponent.h"
#include "../Component/CameraComponent.h"
#include "../Component/TargetArm.h"
#include "../Component/SceneComponent.h"
#include "../Component/ColliderBox2D.h"
#include "../Animation/Animation2D.h"
#include "../UI/UIButton.h"
#include "../UI/UIImage.h"
#include "../UI/UIWindow.h"

CScene::CScene()	:
	m_Change(false),
	m_Start(false)
{
	m_Name = "Scene";

	m_SceneInfo = new CSceneInfo;

	m_SceneInfo->m_Owner = this;

	m_SceneInfo->Init();

	m_SceneResource = new CSceneResource;

	m_SceneResource->m_Owner = this;

	m_SceneResource->Init();

	m_CameraManager = new CCameraManager;

	m_CameraManager->m_Owner = this;

	m_CameraManager->Init();

	m_CollisionManager = new CSceneCollision;

	m_CollisionManager->m_Owner = this;

	m_CollisionManager->Init();

	m_Viewport = new CSceneViewport;

	m_Viewport->m_Owner = this;

	m_Viewport->Init();

	m_NavManager = new CNavigationManager;

	m_NavManager->m_Owner = this;

	m_NavManager->Init();
}

CScene::~CScene()
{
	CInput::GetInst()->ClearCallback(this);

	SAFE_DELETE(m_Viewport);
	SAFE_DELETE(m_CollisionManager);
	SAFE_DELETE(m_CameraManager);
	SAFE_DELETE(m_SceneResource);
	SAFE_DELETE(m_SceneInfo);
}



void CScene::Start()
{
	m_Start = true;

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	m_CameraManager->Start();

	m_Viewport->Start();
}

bool CScene::Init()
{



	return true;
}

void CScene::Update(float DeltaTime)
{
	if (m_SceneInfo)
		m_SceneInfo->Update(DeltaTime);

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(DeltaTime);
		++iter;
	}

	m_CameraManager->Update(DeltaTime);

	m_Viewport->Update(DeltaTime);
}

void CScene::PostUpdate(float DeltaTime)
{
	if (m_SceneInfo)
		m_SceneInfo->PostUpdate(DeltaTime);

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}

	m_CameraManager->PostUpdate(DeltaTime);

	m_Viewport->PostUpdate(DeltaTime);
}

void CScene::Collision(float DeltaTime)
{
	m_CollisionManager->Update(DeltaTime);
}

void CScene::Save(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "wb");

	if (!File)
		return;

	// 이름 저장
	int	Length = (int)m_Name.length();

	fwrite(&Length, 4, 1, File);
	fwrite(m_Name.c_str(), 1, Length, File);

	// SceneInfo 저장
	size_t TypeID = m_SceneInfo->GetTypeID();
	m_SceneInfo->Save(File);

	//매니저는 타입을 명확하게 알고있으므로 TypeID X
	m_CameraManager->Save(File);
	m_CollisionManager->Save(File);
	m_Viewport->Save(File);

	int	ObjCount = (int)m_ObjList.size();
	fwrite(&ObjCount, 4, 1, File);

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		//게임오브젝트의 TypeID를 저장. 불러오기 시 사용
		size_t TypeID = (*iter)->GetTypeID();
		fwrite(&TypeID, sizeof(TypeID), 1, File);

		(*iter)->Save(File);
	}

	fclose(File);
}

void CScene::Load(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "rb");

	if (!File)
		return;

	fseek(File, 0, SEEK_END);

	int	FileSize = (int)ftell(File);

	fseek(File, 0, SEEK_SET);

	int	LoadSize = 0;


	// 이름 저장
	int	Length = 0;
	char	Name[256] = {};

	fread(&Length, 4, 1, File);
	fread(Name, 1, Length, File);

	LoadSize += 4 + Length;

	if (m_LoadingCallback)
		m_LoadingCallback(LoadSize / (float)FileSize);

	m_Name = Name;

	// SceneInfo 불러오기
	SAFE_DELETE(m_SceneInfo);

	size_t TypeID = 0;
	fread(&TypeID, sizeof(size_t), 1, File);

	m_SceneInfo = static_cast<CSceneInfo*>(CCDO::CloneCDO(TypeID));

	m_SceneInfo->m_Owner = this;

	int	CurPos = (int)ftell(File);

	m_SceneInfo->Load(File);

	int	NextPos = (int)ftell(File);

	int	CurLoadSize = NextPos - CurPos;

	if (CurLoadSize > 0)
	{
		LoadSize += CurLoadSize;

		if (m_LoadingCallback)
			m_LoadingCallback(LoadSize / (float)FileSize);
	}

	CurPos = NextPos;

	m_CameraManager->Load(File);

	NextPos = (int)ftell(File);

	CurLoadSize = NextPos - CurPos;

	if (CurLoadSize > 0)
	{
		LoadSize += CurLoadSize;

		if (m_LoadingCallback)
			m_LoadingCallback(LoadSize / (float)FileSize);
	}

	CurPos = NextPos;

	m_CollisionManager->Load(File);

	NextPos = (int)ftell(File);

	CurLoadSize = NextPos - CurPos;

	if (CurLoadSize > 0)
	{
		LoadSize += CurLoadSize;

		if (m_LoadingCallback)
			m_LoadingCallback(LoadSize / (float)FileSize);
	}

	CurPos = NextPos;

	m_Viewport->Load(File);

	NextPos = (int)ftell(File);

	CurLoadSize = NextPos - CurPos;

	if (CurLoadSize > 0)
	{
		LoadSize += CurLoadSize;

		if (m_LoadingCallback)
			m_LoadingCallback(LoadSize / (float)FileSize);
	}

	CurPos = NextPos;

	int	ObjCount = 0;

	fread(&ObjCount, 4, 1, File);

	LoadSize += 4;

	NextPos += 4;

	if (m_LoadingCallback)
		m_LoadingCallback(LoadSize / (float)FileSize);

	CurPos = NextPos;

	for (int i = 0; i < ObjCount; ++i)
	{
		Length = 0;

		char	ObjClassTypeName[256] = {};

		fread(&Length, 4, 1, File);
		fread(ObjClassTypeName, 1, Length, File);


		LoadSize += 4 + Length;
		NextPos += 4 + Length;

		if (m_LoadingCallback)
			m_LoadingCallback(LoadSize / (float)FileSize);

		CurPos = NextPos;

		TypeID = 0;
		fread(&TypeID, sizeof(size_t), 1, File);
		CGameObject* ObjCDO = static_cast<CGameObject*>(CCDO::CloneCDO(TypeID));

		ObjCDO->SetScene(this);

		ObjCDO->Load(File);

		NextPos = (int)ftell(File);

		CurLoadSize = NextPos - CurPos;

		if (CurLoadSize > 0)
		{
			LoadSize += CurLoadSize;

			if (m_LoadingCallback)
				m_LoadingCallback(LoadSize / (float)FileSize);
		}

		CurPos = NextPos;

		m_ObjList.push_back(ObjCDO);
	}

	m_SceneInfo->LoadComplete();

	fclose(File);
}

void CScene::GetAllGameObjectHierarchyName(std::vector<HierarchyObjectName>& vecName)
{
	auto    iter = m_ObjList.begin();
	auto    iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		HierarchyObjectName	Names;

		//CGameObject* Parent = (*iter)->GetParent();

		Names.Name = (*iter)->GetName();
		Names.ClassName = (*iter)->GetObjectTypeName();
		Names.Obj= *iter;
		//Names.Parent = Parent;

		/*if (Parent)
		{
			Names.ParentName = Parent->GetName();
			Names.ParentClassName = Parent->GetComponentTypeName();
		}*/

		vecName.push_back(Names);
	}
}



CGameObject* CScene::FindObject(const std::string& Name)
{
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return *iter;
	}

	return nullptr;
}
