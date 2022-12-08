#include "SCUnitSpriteComponent.h"

//��� ����
#include "../Render/RenderManager.h"
#include "../Resource/Material/Material.h"

//���ҽ� ����
#include "../Resource/ResourceManager.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Animation/Animation2D.h"


//�������, ����ȭ����
#include "../Resource/Shader/SCUnitConstantBuffer.h"
#include "../Resource/Shader/StructuredBuffer.h"

//��İ���
#include "CameraComponent.h"

//���� ������Ʈ
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
	//���� �ڽ� Ŭ�������� ���������� ����س��� �ʾҴٸ� �ٷ� return false
	if (!m_SCUnitInfo)
		return false;

	if (!CSceneComponent::CDOPreload())
		return false;

	//���� �̸� = ���� ������ �ϴ� ���� -> �ϴ� �ý��� ���� �������� �ּ� ó��
	//SetSCUnitInfo(m_FileName);

	//�޽� ���
	m_Mesh = CResourceManager::GetInst()->FindMesh("CenterUVRect");

	m_Transform->Set2D(true);

	//Material ���� �� ���
	m_MaterialSCUnit = CResourceManager::GetInst()->FindMaterial("SCUnitMaterial");

	if (nullptr == m_MaterialSCUnit)
	{
		m_MaterialSCUnit = CResourceManager::GetInst()->CreateMaterial<CMaterial>("SCUnitMaterial");
		m_MaterialSCUnit->SetRenderState("AlphaBlend");

		//������ ���̴� ����
		m_MaterialSCUnit->SetShader("SCUnitShader");
	}

	

	//�ϴ� ���� ������ ����� ����. PrimitiveComponent�� vecMaterial�� ��� ���� ����
	m_vecMaterial.clear();

	

	//�������, ����ȭ���� ����
	m_CBuffer = std::make_shared<CSCUnitConstantBuffer>();
	m_CBuffer->Init();

	//�̸� ���� Ȯ��
	m_MaterialSCUnit->ReservevecTexInfo((int)ESCUnit_TextureLayer::End);
	//�÷��� ��ȸ�ϸ鼭 ���ΰͳ��� ���ϰ� ������ ���̾� ������ ������ۿ� ���.
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
		//������� ���� ��� �ش� �ִϸ��̼��� ���
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

	//�ν��Ͻ����� ������ ����Ϸ��� �� �ѹ��� ����ȭ���ۿ� ����س����� ��
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
	//�� ����Ŭ ���� ���� ����ȭ ���۸� �ʱ�ȭ
	m_PrivateSBuffer.SCUnit_SBufferFlag = (unsigned int)0;

	CSceneComponent::Update(DeltaTime);

	m_PrevAction = m_CurrentAction;

	//�ִϸ��̼� ������Ʈ �� ���� �׼��� ���
	ComputeCurrentAction();

	for (int i = 0; i < (int)ESCUnit_TextureLayer::End; ++i)
	{
		if (m_vecAnimLayer[i])
		{
			m_vecAnimLayer[i]->Update(DeltaTime);

			//���⿡ ���� ȸ�� ó��
			unsigned int RenderFlag = m_CBuffer->GetRenderFlags();
			int Dir = (int)m_SCUnitRoot->GetDirection();
			if (Dir > 16)
			{
				Dir = 31 - Dir;
				m_PrivateSBuffer.SCUnit_SBufferFlag |= ESCUnit_SBufferFlag::ESCUnitMainXFlip;
			}

			const Animation2DFrameData* Data =
				m_vecAnimLayer[i]->GetCurrentAnimationFrameDataSCUnit(Dir);



			//����� �ִϸ��̼��� �ִ��� Ȯ���ϰ� ���� ��� �׷������� �ؽ�ó ��ȣ�� ���
			//������ �׳� ���������
			if (Data)
			{
				//����� �ؽ�ó �����¸�ŭ ��Ʈ�� �������� �̵���Ų ������ ��Ʈ�÷��׸� ���ָ� �ȴ�.
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


	//�ν��Ͻ��� ���� ��ġ������ �ڽ��� ����ü�� ����
	const Matrix& matWorld = m_Transform->GetWorldMatrix();
	const Matrix& matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	const Matrix& matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();

	//����-��-������� ���
	m_PrivateSBuffer.matWVP = matWorld * matView * matProj;
	m_PrivateSBuffer.matWVP.Transpose();

	m_PrivateSBuffer.Pivot = m_Transform->GetPivot();
	m_PrivateSBuffer.MeshSize = m_Transform->GetMeshSize();

	//���� �׼ǿ� ���� �����÷��� ���(�ӽ�)
	//TurnOnSBufferFlag(ESCUnitFlagAll);
}

void CSCUnitSpriteComponent::Render()
{
	CComponent::Render();

	
	//���� ����ȭ ���� �����͸� ���
	m_SBufferInfo->AddBuffer(m_PrivateSBuffer);

	//�Ź� �����÷��׸� �ʱ�ȭ(Update()�� �ű�)
	//m_PrivateSBuffer.SCUnit_SBufferFlag = 0u;


	////����ȭ ���� ä��� ����(���� Update�� �ű�)
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

	//���� �����Ӱ� �׼��� �޶����� ��� �ؾ� �� �۾��� ����
	if (m_PrevAction != m_CurrentAction)
	{
		//���⿡ ������ �ۼ�
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

	//���⿡ ���߿� �ִϸ��̼� ��� ������ �߰��ؾ� ��
	if (nullptr == Seq)
		return;

	m_vecAnimLayer[Bind.TexLayer]->AddAnimation(Bind.AnimName, Seq);

	//�ؽ�ó�� ������ ���
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

	//���ε��� ���
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

	//�̹� �����Ǿ� ���� ��� return
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


