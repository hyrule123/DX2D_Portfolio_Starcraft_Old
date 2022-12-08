#include "SCUnitSpriteComponent.h"

//출력 관련
#include "../Render/RenderManager.h"
#include "../Resource/Material/Material.h"

//리소스 관련
#include "../Resource/ResourceManager.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Animation/Animation2D.h"


//상수버퍼, 구조화버퍼
#include "../Resource/Shader/SCUnitConstantBuffer.h"
#include "../Resource/Shader/StructuredBuffer.h"

//행렬계산용
#include "CameraComponent.h"

//상위 컴포넌트
#include "SCUnitRootComponent.h"




CSCUnitSpriteComponent::CSCUnitSpriteComponent():
	m_CBuffer(),
	m_PrivateSBuffer{},
	m_PrevAction(ESCUnit_Actions::None),
	m_CurrentAction(ESCUnit_Actions::Birth),
	m_vecAnimLayer{},
	m_SCUnitRoot(),
	m_Selected(),
	m_SCUnitInfo(),
	m_vecTexLayer{},
	m_vecActionAnimBind{}
	//m_TexLayerIdxFlagBinding{}
{
	m_ComponentTypeName = "SCUnitSpriteComponent";
}

CSCUnitSpriteComponent::CSCUnitSpriteComponent(const CSCUnitSpriteComponent& component) :
	CSpriteComponent(component),
	m_CBuffer(component.m_CBuffer),
	m_SBufferInfo(component.m_SBufferInfo),
	m_PrivateSBuffer(component.m_PrivateSBuffer),
	m_PrevAction(component.m_PrevAction),
	m_CurrentAction(component.m_CurrentAction),
	m_SCUnitInfo(component.m_SCUnitInfo)
{
	if(component.m_MaterialSCUnit)
		m_MaterialSCUnit = component.m_MaterialSCUnit->Clone();

	for (int i = 0; i < (int)ESCUnit_TextureLayer::End; ++i)
	{
		if(component.m_vecAnimLayer[i])
			m_vecAnimLayer[i] = component.m_vecAnimLayer[i]->Clone();
	}

	for (int i = 0; i < (int)ESCUnit_TextureLayer::End; ++i)
	{
		m_vecTexLayer[i] = component.m_vecTexLayer[i];
	}

	for (int i = 0; i < (int)ESCUnit_Actions::End; ++i)
	{
		m_vecActionAnimBind[i] = component.m_vecActionAnimBind[i];
	}

	/*size_t size = sizeof(ESCUnit_TextureLayerFlag) * (size_t)(int)ESCUnit_TextureLayer::End;
	memcpy_s(m_TexLayerIdxFlagBinding, size, component.m_TexLayerIdxFlagBinding, size);*/
}

CSCUnitSpriteComponent::~CSCUnitSpriteComponent()
{
	for (int i = 0; i < (int)ESCUnit_TextureLayer::End; ++i)
	{
		SAFE_DELETE(m_vecAnimLayer[i]);
	}

	if (m_ObjStatus == EObjStatus::PLO)
		SAFE_DELETE(m_SCUnitInfo);
}

bool CSCUnitSpriteComponent::CDOPreload()
{
	//만약 자식 클래스에서 유닛정보를 등록해놓지 않았다면 바로 return false
	if (!m_SCUnitInfo)
		return false;

	if (!CSceneComponent::CDOPreload())
		return false;

	//파일 이름 = 유닛 정보로 일단 설정 -> 일단 시스템 구축 전까지는 주석 처리
	//SetSCUnitInfo(m_FileName);

	//메쉬 등록
	m_Mesh = CResourceManager::GetInst()->FindMesh("CenterUVRect");

	m_Transform->Set2D(true);

	//Material 생성 및 등록
	m_MaterialSCUnit = CResourceManager::GetInst()->FindMaterial("SCUnitMaterial");

	if (nullptr == m_MaterialSCUnit)
	{
		m_MaterialSCUnit = CResourceManager::GetInst()->CreateMaterial<CMaterial>("SCUnitMaterial");
		m_MaterialSCUnit->SetRenderState("AlphaBlend");

		//재질에 쉐이더 지정
		m_MaterialSCUnit->SetShader("SCUnitShader");
	}

	

	//일단 단일 재질을 사용할 예정. PrimitiveComponent의 vecMaterial은 사용 안할 것임
	m_vecMaterial.clear();

	

	//상수버퍼, 구조화버퍼 생성
	m_CBuffer = std::make_shared<CSCUnitConstantBuffer>();
	m_CBuffer->Init();

	//미리 공간 확보
	m_MaterialSCUnit->ReservevecTexInfo((int)ESCUnit_TextureLayer::End);
	//플래그 순회하면서 참인것끼리 비교하고 유닛의 레이어 정보를 상수버퍼에 등록.
	for (UINT8 i = 0; i < (UINT8)ESCUnit_TextureLayerFlag::FlagCount; ++i)
	{
		UINT8 Flag = (UINT8)1 << i;
		if (m_SCUnitInfo->UsingTextureLayerFlags & Flag)
		{
			ESCUnit_TextureLayerFlag TexLayer = (ESCUnit_TextureLayerFlag)(Flag);

			switch (TexLayer)
			{
			case ESCUnit_TextureLayerFlag::Selected:
				m_CBuffer->TurnOnRenderFlags(ESCUnit_TextureLayerFlag::Selected);
				CreateNewAnimLayer(ESCUnit_TextureLayer::Selected);
				//m_MaterialSCUnit->AddTextureEmpty((int)ESCUnit_TextureLayer::Selected, (int)EShaderBufferType::Pixel);
				break;
			case ESCUnit_TextureLayerFlag::Main:
				m_CBuffer->TurnOnRenderFlags(ESCUnit_TextureLayerFlag::Main);
				CreateNewAnimLayer(ESCUnit_TextureLayer::MainShadow);
				//m_MaterialSCUnit->AddTextureEmpty((int)ESCUnit_TextureLayer::MainShadow, (int)EShaderBufferType::Pixel);
				break;


			case ESCUnit_TextureLayerFlag::Top:
				m_CBuffer->TurnOnRenderFlags(ESCUnit_TextureLayerFlag::Top);
				CreateNewAnimLayer(ESCUnit_TextureLayer::Top);
				//m_MaterialSCUnit->AddTextureEmpty((int)ESCUnit_TextureLayer::Top, (int)EShaderBufferType::Pixel);
				break;
			case ESCUnit_TextureLayerFlag::Effect:
				m_CBuffer->TurnOnRenderFlags(ESCUnit_TextureLayerFlag::Effect);
				CreateNewAnimLayer(ESCUnit_TextureLayer::Effect);
				//m_MaterialSCUnit->AddTextureEmpty((int)ESCUnit_TextureLayer::Effect, (int)EShaderBufferType::Pixel);
				break;

			case ESCUnit_TextureLayerFlag::Booster:
				m_CBuffer->TurnOnRenderFlags(ESCUnit_TextureLayerFlag::Booster);
				CreateNewAnimLayer(ESCUnit_TextureLayer::Booster);
				//m_MaterialSCUnit->AddTextureEmpty((int)ESCUnit_TextureLayer::Booster, (int)EShaderBufferType::Pixel);
				break;

			case ESCUnit_TextureLayerFlag::UseShadowSprite:
				m_CBuffer->TurnOnRenderFlags(ESCUnit_TextureLayerFlag::UseShadowSprite);
				break;
			default:
				break;
			}
		}
	}


	for (int i = 0; i < (int)ESCUnit_Actions::End; ++i)
	{
		//비어있지 않을 경우 해당 애니메이션을 등록
		if (0 != strlen((m_SCUnitInfo->AnimationNames[i])))
		{
			AddActionAnimation((ESCUnit_Actions)i, m_SCUnitInfo->AnimationNames[i]);
		}
	}
	

	/*	
	m_SBuffer = new CStructuredBuffer;
	m_SBuffer->Init("SCUnistSBuffer", sizeof(SCUnit_SBuffer), 2000, 8, false, (int)EShaderBufferType::Vertex);
	*/

	m_SBufferInfo = new CSharedStructuredBuffer<SCUnit_SBuffer>;

	m_SBufferInfo->Init("SCUnit_SBuffer", 400, 6, true, (int)EShaderBufferType::Vertex);

	//인스턴싱으로 유닛을 출력하려면 딱 한번만 구조화버퍼에 등록해놓으면 됨
	CRenderManager::GetInst()->AddInstancingMap(this);


	return true;
}

void CSCUnitSpriteComponent::Start()
{
	CSceneComponent::Start();

#ifdef _DEBUG
	if (!m_Parent)
		assert(0);

	m_SCUnitRoot = static_cast<CSCUnitRootComponent*>(m_Parent);
#endif

	m_vecAnimLayer[(int)ESCUnit_TextureLayer::MainShadow]->Start();

	CRenderManager::GetInst()->AddRenderList(this);
}

bool CSCUnitSpriteComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	for (int i = 0; i < (int)ESCUnit_TextureLayer::End; ++i)
	{
		if(m_vecAnimLayer[i])
			m_vecAnimLayer[i]->SetOwner(this);
	}

	return true;
}

void CSCUnitSpriteComponent::Update(float DeltaTime)
{
	//매 사이클 마다 개인 구조화 버퍼를 초기화
	m_PrivateSBuffer.SCUnit_SBufferFlag = (unsigned int)0;

	CSceneComponent::Update(DeltaTime);

	m_PrevAction = m_CurrentAction;

	//애니메이션 업데이트 전 현재 액션을 계산
	ComputeCurrentAction();

	for (int i = 0; i < (int)ESCUnit_TextureLayer::End; ++i)
	{
		if (m_vecAnimLayer[i])
		{
			m_vecAnimLayer[i]->Update(DeltaTime);

			//방향에 따라서 회전 처리
			unsigned int RenderFlag = m_CBuffer->GetRenderFlags();
			int Dir = (int)m_SCUnitRoot->GetDirection();
			if (Dir > 16)
			{
				Dir = 31 - Dir;
				m_PrivateSBuffer.SCUnit_SBufferFlag |= ESCUnit_SBufferFlag::ESCUnitMainXFlip;
			}

			const Animation2DFrameData* Data =
				m_vecAnimLayer[i]->GetCurrentAnimationFrameDataSCUnit(Dir);



			//출력할 애니메이션이 있는지 확인하고 있을 경우 그려내야할 텍스처 번호를 등록
			//없으면 그냥 지나가면됨
			if (Data)
			{
				//저장된 텍스처 프리셋만큼 비트를 왼쪽으로 이동시킨 값으로 비트플래그를 켜주면 된다.
				m_PrivateSBuffer.SCUnit_SBufferFlag |= 1 << i;
				m_PrivateSBuffer.SCUnit_SBufferTexFrameInfo[i].Start = Data->Start;
				m_PrivateSBuffer.SCUnit_SBufferTexFrameInfo[i].End = Data->End;
			}
		}
	}
}

void CSCUnitSpriteComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);


	//인스턴싱을 위한 위치정보를 자신의 구조체에 삽입
	const Matrix& matWorld = m_Transform->GetWorldMatrix();
	const Matrix& matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	const Matrix& matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();

	//월드-뷰-투영행렬 계산
	m_PrivateSBuffer.matWVP = matWorld * matView * matProj;
	m_PrivateSBuffer.matWVP.Transpose();

	m_PrivateSBuffer.Pivot = m_Transform->GetPivot();
	m_PrivateSBuffer.MeshSize = m_Transform->GetMeshSize();

	//현재 액션에 따라서 렌더플래그 계산(임시)
	//TurnOnSBufferFlag(ESCUnitFlagAll);
}

void CSCUnitSpriteComponent::Render()
{
	CComponent::Render();

	
	//개인 구조화 버퍼 데이터를 등록
	m_SBufferInfo->AddBuffer(m_PrivateSBuffer);

	//매번 렌더플래그를 초기화(Update()로 옮김)
	//m_PrivateSBuffer.SCUnit_SBufferFlag = 0u;


	////구조화 버퍼 채우기 과정(현재 Update로 옮김)
	//for (int i = 0; i < (int)ESCUnit_TextureLayer::End; ++i)
	//{
	//	CAnimation2D* AnimLayer = m_vecAnimLayer[i];

	//	if (AnimLayer->IsPlaying())
	//	{
	//		m_PrivateSBuffer.SCUnit_SBufferFlag |= ESCUnit_SBufferFlag::ESCUnitMainRender;

	//		const Animation2DFrameData* Data = AnimLayer->GetCurrentAnimationFrameDataSCUnit(Dir);
	//		if (!Data)
	//			break;

	//		m_PrivateSBuffer.SCUnit_SBufferTexFrameInfo[i].Start = Data->Start;
	//		m_PrivateSBuffer.SCUnit_SBufferTexFrameInfo[i].End = Data->End;
	//	}
	//}
}

void CSCUnitSpriteComponent::RenderInstanced()
{
	int BufferCount = m_SBufferInfo->GetInstancingBufferCount();

	if (m_Animation)
		m_Animation->SetShader();

	else
	{
		CAnimation2DConstantBuffer* Buffer = CResourceManager::GetInst()->GetAnim2DConstantBuffer();

		Buffer->SetAnim2DEnable(false);

		Buffer->UpdateBuffer();
	}

	CComponent::Render();

	m_MaterialSCUnit->SetMaterialInstanced();
	m_SBufferInfo->UpdateBuffer();
	m_SBufferInfo->SetShader();

	m_Mesh->RenderInstancing(m_SBufferInfo->GetInstancingBufferCount());

	m_SBufferInfo->ResetShader();
	m_MaterialSCUnit->ResetMaterialInstanced();
}

CSCUnitSpriteComponent* CSCUnitSpriteComponent::Clone() const
{
	return new CSCUnitSpriteComponent(*this);
}

void CSCUnitSpriteComponent::Save(FILE* File)
{
	CSpriteComponent::Save(File);
}

void CSCUnitSpriteComponent::Load(FILE* File)
{
	CSpriteComponent::Load(File);
}

bool CSCUnitSpriteComponent::SetTexture(CTexture* Texture, int Index)
{
	m_MaterialSCUnit->SetTextureSCUnit(Texture,Index);

	return true;
}

void CSCUnitSpriteComponent::SetSCUnitInfo(const std::string& UnitName)
{
	m_SCUnitInfo = CResourceManager::GetInst()->GetCloneSCUnitInfo(UnitName);
}

void CSCUnitSpriteComponent::ComputeCurrentAction()
{
	m_CurrentAction = ESCUnit_Actions::Move;

	//이전 프레임과 액션이 달라졌을 경우 해야 할 작업을 지정
	if (m_PrevAction != m_CurrentAction)
	{
		//여기에 로직을 작성
	}
}

void CSCUnitSpriteComponent::TurnOnSBufferFlag(ESCUnit_SBufferFlag Flag)
{
	m_PrivateSBuffer.SCUnit_SBufferFlag |= Flag;
}

void CSCUnitSpriteComponent::TurnOffSBufferFlag(ESCUnit_SBufferFlag Flag)
{
	m_PrivateSBuffer.SCUnit_SBufferFlag &= ~Flag;
}

void CSCUnitSpriteComponent::AddActionAnimation(ESCUnit_Actions ENumAction, const std::string& AnimationName)
{
	SCUnit_ActionAnimBind Bind = {};
	Bind.ActionNum = (int)ENumAction;
	Bind.AnimName = AnimationName;


	switch (ENumAction)
	{
	case ESCUnit_Actions::Birth:
	case ESCUnit_Actions::Death:
	case ESCUnit_Actions::Idle:
	case ESCUnit_Actions::Move:
	case ESCUnit_Actions::SurfaceAttack:
	case ESCUnit_Actions::AirAttack:
	case ESCUnit_Actions::Debris:
	case ESCUnit_Actions::Construction:
	case ESCUnit_Actions::ConstructionComplete:
		Bind.TexLayer = (int)ESCUnit_TextureLayer::MainShadow;
		break;
	case ESCUnit_Actions::BuildUnit:
		Bind.TexLayer = (int)ESCUnit_TextureLayer::Top;
		break;
	default:
		break;
	}

	CAnimationSequence2D* Seq = CResourceManager::GetInst()->FindAnimationSequence2D(Bind.AnimName);

	//여기에 나중에 애니메이션 등록 과정도 추가해야 함
	if (nullptr == Seq)
		return;

	m_vecAnimLayer[Bind.TexLayer]->AddAnimation(Bind.AnimName, Seq);

	//텍스처를 재질에 등록
	CTexture* Tex = Seq->GetTexture();

	if ((int)ESCUnit_TextureLayer::MainShadow == Bind.TexLayer &&
		EImageType::Array == Tex->GetImageType() &&
		1 < Tex->GetImageCount()
		)
	{
		m_CBuffer->TurnOnRenderFlags(ESCUnit_TextureLayerFlag::UseShadowSprite);
	}
	
	m_MaterialSCUnit->SetTextureSCUnit(Tex, Bind.TexLayer);
	//m_MaterialSCUnit->AddTexture(Bind.TexLayer, (int)EShaderBufferType::Pixel, Bind.AnimName, Tex);

	//바인딩을 등록
	m_vecActionAnimBind[Bind.ActionNum] = Bind;
}

void CSCUnitSpriteComponent::ChangeAnimationByAction(ESCUnit_Actions ENumAction)
{
	const SCUnit_ActionAnimBind& Bind = m_vecActionAnimBind[(int)ENumAction];

	if (-1 == Bind.TexLayer)
		return;

	m_vecAnimLayer[Bind.TexLayer]->ChangeAnimation(Bind.AnimName);

	//switch (ENumAction)
	//{
	//case ESCUnit_Actions::Birth:
	//	m_vecAnimLayer[iter->second.AnimLayer]->ChangeAnimation(iter->second.AnimName);
	//	break;
	//case ESCUnit_Actions::Idle:
	//	m_vecAnimLayer[AnimLayer_Shadow_Main]->ChangeAnimation(iter->second);
	//	break;
	//case ESCUnit_Actions::Move:
	//	m_vecAnimLayer[AnimLayer_Shadow_Main]->ChangeAnimation(iter->second);
	//	break;
	//case ESCUnit_Actions::SurfaceAttack:
	//	break;
	//case ESCUnit_Actions::AirAttack:
	//	break;
	//case ESCUnit_Actions::Death:
	//	break;
	//case ESCUnit_Actions::Debris:
	//	break;
	//case ESCUnit_Actions::Construction:
	//	break;
	//case ESCUnit_Actions::ConstructionComplete:
	//	break;
	//case ESCUnit_Actions::BuildUnit:
	//	break;
	//default:
	//	break;
	//}


}

void CSCUnitSpriteComponent::CreateNewAnimLayer(ESCUnit_TextureLayer Layer)
{
	int L = (int)Layer;

	//이미 생성되어 있을 경우 return
	if (m_vecAnimLayer[L] || !m_MaterialSCUnit)
		return;

	m_vecAnimLayer[L] = new CAnimation2D;
	m_vecAnimLayer[L]->Init();
	m_vecAnimLayer[L]->SetInstancing(true);
}

void CSCUnitSpriteComponent::RegisterSequence()
{
	//for (auto& iter : *m_vecActionAnim)
	//{
	//	m_vecAnimLayer[iter.second.AnimLayer]->AddAnimation(iter.second.AnimName, iter.second.AnimName);
	//}
}

CAnimation2D* CSCUnitSpriteComponent::GetUnitAnimLayer(int Index)
{
	//if (AnimLayer_Max <= Index)
	//	return nullptr;

	//return m_vecAnimLayer[Index];

	return nullptr;
}

//bool CSCUnitSpriteComponent::AddUnitAnimation(ESCUnit_TextureLayerFlag Layer, const std::string& Name, const std::string& SequenceName, float PlayTime, float PlayScale, bool Loop, bool Reverse)
//{
//	if (SequenceName.empty())
//		return false;
//
//	CAnimationSequence2D* Seq = CResourceManager::GetInst()->FindAnimationSequence2D(Name);
//
//	return AddUnitAnimation(Layer, Name, Seq, PlayTime, PlayScale, Loop, Reverse);
//}

//bool CSCUnitSpriteComponent::AddUnitAnimation(ESCUnit_TextureLayerFlag Layer, const std::string& Name, CAnimationSequence2D* Sequence, float PlayTime, float PlayScale, bool Loop, bool Reverse)
//{
//	if (!Sequence)
//		return false;
//
//	switch (Layer)
//	{
//	case ESCUnit_TextureLayerFlag::UnitMainWithShadow:
//		m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::UseShadowSprite);
//	case ESCUnit_TextureLayerFlag::UnitMainOnly:
//		m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::MainShadow);
//		m_vecAnimLayer[AnimLayer_Shadow_Main]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
//		break;
//	case ESCUnit_TextureLayerFlag::Top:
//		m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::MainShadow);
//		m_vecAnimLayer[AnimLayer_Top]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
//		break;
//	case ESCUnit_TextureLayerFlag::Effect:
//		m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::MainShadow);
//		m_vecAnimLayer[AnimLayer_Top]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
//		break;
//	case ESCUnit_TextureLayerFlag::Attack:
//		m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::MainShadow);
//		m_vecAnimLayer[AnimLayer_Top]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
//		break;
//	case ESCUnit_TextureLayerFlag::UnitBoost:
//		m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::MainShadow);
//		m_vecAnimLayer[AnimLayer_Top]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
//		break;
//	default:
//		break;
//	}
//
//
//	return m_vecAnimLayer[(int)Layer]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
//}


