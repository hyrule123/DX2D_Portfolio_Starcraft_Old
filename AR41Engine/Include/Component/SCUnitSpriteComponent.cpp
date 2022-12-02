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
	m_PrivateSBuffer{},
	m_PrevAction(ESCUnit_Actions::None),
	m_CurrentAction(ESCUnit_Actions::Birth),
	m_AnimationUnitLayer{},
	m_SCUnitRoot(),
	m_Selected()
{
	m_ComponentTypeName = "SCUnitSpriteComponent";
}

CSCUnitSpriteComponent::CSCUnitSpriteComponent(const CSCUnitSpriteComponent& component) :
	CSpriteComponent(component),
	m_SBufferInfo(component.m_SBufferInfo),
	m_PrivateSBuffer(component.m_PrivateSBuffer),
	m_mapUnitActions(component.m_mapUnitActions),
	m_PrevAction(component.m_PrevAction),
	m_CurrentAction(component.m_CurrentAction)
{
	m_Material = component.m_Material->Clone();

	for (int i = 0; i < AnimLayer_Max; ++i)
	{
		if(m_AnimationUnitLayer[i])
			m_AnimationUnitLayer[i] = component.m_AnimationUnitLayer[i]->Clone();
	}
}

CSCUnitSpriteComponent::~CSCUnitSpriteComponent()
{
	for (int i = 0; i < AnimLayer_Max; ++i)
	{
		SAFE_DELETE(m_AnimationUnitLayer[i]);
	}

}

bool CSCUnitSpriteComponent::CDOPreload()
{
	if (!CSceneComponent::CDOPreload())
		return false;

	if (!m_SCUnitRoot)
		assert(0);

	//�޽� ���
	m_Mesh = CResourceManager::GetInst()->FindMesh("CenterUVRect");

	m_Transform->Set2D(true);

	//Material ���� �� ���
	m_Material = CResourceManager::GetInst()->CreateMaterial<CMaterial>("SCUnitMaterial");

	//�ϴ� ���� ������ ����� ����. PrimitiveComponent�� vecMaterial�� ��� ���� ����
	m_vecMaterial.clear();

	//������ ���̴� ����
	m_Material->SetShader("SCUnitShader");

	//�������, ����ȭ���� ����
	m_CBuffer = std::make_shared<CSCUnitConstantBuffer>();
	m_CBuffer->Init();


	//�÷��� ��ȸ�ϸ鼭 ���ΰͳ��� ���ϰ� ������ ���̾� ������ ������ۿ� ���.
	//����ϴ� ���̾��� �ִϸ��̼� ���̾ �����Ҵ��ϰ� �ʱ�ȭ
	//��ӹ޴� Ŭ�������� �ݵ�� ���� ������Ʈ�� ����س��� ��
	if (!m_SCUnitRoot)
		assert(0);

	SCUnitInfo* UnitInfo = m_SCUnitRoot->GetSCUnitInfo();

	for (UINT8 i = 0; i < (UINT8)ESCUnit_TextureLayer::Count; ++i)
	{
		UINT8 Flag = (UINT8)1 << i;
		if (UnitInfo->UsingTextureLayerFlags & Flag)
		{
			ESCUnit_TextureLayer TexLayer = (ESCUnit_TextureLayer)(Flag);

			switch (TexLayer)
			{
			case ESCUnit_TextureLayer::MainWithShadow:
				m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::UseShadowSprite);
			case ESCUnit_TextureLayer::MainOnly:
				m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::MainShadow);
				CreateNewAnimLayer(ESCUnit_AnimLayer::AnimLayer_Shadow_Main);
				break;


			case ESCUnit_TextureLayer::Top:
				m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::Top);
				CreateNewAnimLayer(ESCUnit_AnimLayer::AnimLayer_Top);
				break;
			case ESCUnit_TextureLayer::Effect:
				m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::Effect);
				CreateNewAnimLayer(ESCUnit_AnimLayer::AnimLayer_Effect);
				break;


			case ESCUnit_TextureLayer::Attack:
				m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::Attack);
			case ESCUnit_TextureLayer::Boost:
				m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::Booster);
				CreateNewAnimLayer(ESCUnit_AnimLayer::AnimLayer_Attack_Boost);
				break;
			default:
				break;
			}
		}
	}

	


	/*	
	m_SBuffer = new CStructuredBuffer;
	m_SBuffer->Init("SCUnistSBuffer", sizeof(SCUnit_SBuffer), 2000, 8, false, (int)EShaderBufferType::Vertex);
	*/

	m_SBufferInfo = new CSharedStructuredBuffer<SCUnit_SBuffer>;
	m_SBufferInfo->Init("SCUnit_SBuffer", sizeof(SCUnit_SBuffer), 400, true, (int)EShaderBufferType::Vertex);

	//�ν��Ͻ����� ������ ����Ϸ��� �� �ѹ��� ����ȭ���ۿ� ����س����� ��
	CRenderManager::GetInst()->AddInstancingMap(this);

	//�׼ǿ� ���� �ִϸ��̼� �̸��� �����Ѵ�.
	m_mapUnitActions = std::make_shared<std::unordered_map<ESCUnit_Actions, SCUnit_Anim_LayerNameBind>>();


	return true;
}

void CSCUnitSpriteComponent::Start()
{
	CSceneComponent::Start();

	for (int i = 0; i < AnimLayer_Max; ++i)
	{
		m_AnimationUnitLayer[i]->Start();
	}

	CRenderManager::GetInst()->AddRenderList(this);
}

bool CSCUnitSpriteComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	for (int i = 0; i < AnimLayer_Max; ++i)
	{
		m_AnimationUnitLayer[i]->SetOwner(this);
	}

	return true;
}

void CSCUnitSpriteComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	m_PrevAction = m_CurrentAction;

	//�ִϸ��̼� ������Ʈ �� ���� �׼��� ���
	ComputeCurrentAction();




	//if (m_Animation)
	//	m_Animation->Update(DeltaTime);
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
	TurnOnSBufferFlag(ESCUnitFlagAll);
}

void CSCUnitSpriteComponent::Render()
{
	CComponent::Render();

	//�Ź� �����÷��׸� �ʱ�ȭ
	m_PrivateSBuffer.SCUnit_SBufferFlag = 0u;

	unsigned int RenderFlag = m_CBuffer->GetRenderFlags();
	int Dir = (int)m_SCUnitRoot->GetDirection();
	if (Dir > 16)
	{
		Dir = 31 - Dir;
		m_PrivateSBuffer.SCUnit_SBufferFlag |= ESCUnit_SBufferFlag::ESCUnitMainXFlip;
	}


	//����ȭ ���� ä��� ����
	for (int i = 0; i < AnimLayer_Max; ++i)
	{
		switch (i)
		{
		case AnimLayer_Shadow_Main:
			if (RenderFlag & ESCUnit_CBufferRenderFlag::MainShadow)
			{
				CAnimation2D* AnimLayer = m_AnimationUnitLayer[AnimLayer_Shadow_Main];

				if (AnimLayer->IsPlaying())
				{
					m_PrivateSBuffer.SCUnit_SBufferFlag |= ESCUnit_SBufferFlag::ESCUnitMainRender;

					const Animation2DFrameData* Data = AnimLayer->GetCurrentAnimationFrameDataSCUnit(Dir);
					if (!Data)
						break;



					m_PrivateSBuffer.MainShadowStart = Data->Start;
					m_PrivateSBuffer.MainShadowEnd = Data->End;

					CTexture* Tex = AnimLayer->GetCurrentAnimation()->GetTexture();
					m_PrivateSBuffer.MainShadowWidth = (float)Tex->GetWidth();
					m_PrivateSBuffer.MainShadowHeight = (float)Tex->GetHeight();
				}
			}
			break;
		case AnimLayer_Top:
			if (RenderFlag & ESCUnit_CBufferRenderFlag::Top)
			{
				CAnimation2D* AnimLayer = m_AnimationUnitLayer[AnimLayer_Top];

				if (AnimLayer->IsPlaying())
				{
					m_PrivateSBuffer.SCUnit_SBufferFlag |= ESCUnit_SBufferFlag::ESCUnitTopRender;

					const Animation2DFrameData* Data = AnimLayer->GetCurrentAnimationFrameDataSCUnit(Dir);
					if (!Data)
						break;

					m_PrivateSBuffer.TopStart = Data->Start;
					m_PrivateSBuffer.TopEnd = Data->End;

					CTexture* Tex = AnimLayer->GetCurrentAnimation()->GetTexture();
					m_PrivateSBuffer.TopWidth = (float)Tex->GetWidth();
					m_PrivateSBuffer.TopHeight = (float)Tex->GetHeight();
				}
			}
			break;
		case AnimLayer_Effect:
			if (RenderFlag & ESCUnit_CBufferRenderFlag::Effect)
			{
				CAnimation2D* AnimLayer = m_AnimationUnitLayer[Effect];

				if (AnimLayer->IsPlaying())
				{
					m_PrivateSBuffer.SCUnit_SBufferFlag |= ESCUnit_SBufferFlag::ESCUnitEffectRender;

					const Animation2DFrameData* Data = AnimLayer->GetCurrentAnimationFrameDataSCUnit(Dir);
					if (!Data)
						break;

					m_PrivateSBuffer.EffectStart = Data->Start;
					m_PrivateSBuffer.EffectEnd = Data->End;

					CTexture* Tex = AnimLayer->GetCurrentAnimation()->GetTexture();
					m_PrivateSBuffer.EffectWidth = (float)Tex->GetWidth();
					m_PrivateSBuffer.EffectHeight = (float)Tex->GetHeight();
				}
			}
			break;
		case AnimLayer_Attack_Boost:
			if (RenderFlag & (ESCUnit_CBufferRenderFlag::Attack | ESCUnit_CBufferRenderFlag::Booster))
			{

				CAnimation2D* AnimLayer = m_AnimationUnitLayer[AnimLayer_Attack_Boost];

				if (AnimLayer->IsPlaying())
				{
					if (RenderFlag & (ESCUnit_CBufferRenderFlag::Attack))
						m_PrivateSBuffer.SCUnit_SBufferFlag |= ESCUnit_SBufferFlag::ESCUnitAttackRender;
					if (RenderFlag & (ESCUnit_CBufferRenderFlag::Booster))
						m_PrivateSBuffer.SCUnit_SBufferFlag |= ESCUnit_SBufferFlag::ESCUnitBoosterRender;

					const Animation2DFrameData* Data = AnimLayer->GetCurrentAnimationFrameDataSCUnit(Dir);
					if (!Data)
						break;

					m_PrivateSBuffer.AttackBoosterStart = Data->Start;
					m_PrivateSBuffer.AttackBoosterEnd = Data->End;

					CTexture* Tex = AnimLayer->GetCurrentAnimation()->GetTexture();
					m_PrivateSBuffer.AttackBoosterWidth = (float)Tex->GetWidth();
					m_PrivateSBuffer.AttackBoosterHeight = (float)Tex->GetHeight();
				}
			}
			break;
		case AnimLayer_Max:

			break;
		default:
			break;
		}
	}

}

void CSCUnitSpriteComponent::RenderInstanced()
{
	if (m_Animation)
		m_Animation->SetShader();

	else
	{
		CAnimation2DConstantBuffer* Buffer = CResourceManager::GetInst()->GetAnim2DConstantBuffer();

		Buffer->SetAnim2DEnable(false);

		Buffer->UpdateBuffer();
	}

	CSceneComponent::Render();

	int	Size = (int)m_vecMaterial.size();

	for (int i = 0; i < Size; ++i)
	{
		m_vecMaterial[i]->SetMaterial();

		m_Mesh->Render(i);

		m_vecMaterial[i]->ResetMaterial();
	}
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
	m_Material->SetTexture(Texture, Index);

	return true;
}

void CSCUnitSpriteComponent::ComputeCurrentAction()
{
	m_CurrentAction = ESCUnit_Actions::Move;

	//���� �����Ӱ� �׼��� �޶����� ��� �ؾ� �� �۾��� ����
	if (m_PrevAction != m_CurrentAction)
	{
		auto iter = m_mapUnitActions->find(m_CurrentAction);


		switch (m_CurrentAction)
		{
		case ESCUnit_Actions::Birth:
		{

			break;
		}

		case ESCUnit_Actions::Idle:
			break;
		case ESCUnit_Actions::Move:
			break;
		case ESCUnit_Actions::SurfaceAttack:
			break;
		case ESCUnit_Actions::AirAttack:
			break;
		case ESCUnit_Actions::Death:
			break;
		case ESCUnit_Actions::Debris:
			break;
		case ESCUnit_Actions::Construction:
			break;
		case ESCUnit_Actions::ConstructionComplete:
			break;
		case ESCUnit_Actions::BuildUnit:
			break;
		default:
			break;
		}
	}
	//��� ���� �׼��� ��� �� �۾��� ���⿡ �ۼ�
	else
	{
		switch (m_CurrentAction)
		{
		case ESCUnit_Actions::Birth:
			break;
		case ESCUnit_Actions::Idle:
			break;
		case ESCUnit_Actions::Move:
			break;
		case ESCUnit_Actions::SurfaceAttack:
			break;
		case ESCUnit_Actions::AirAttack:
			break;
		case ESCUnit_Actions::Death:
			break;
		case ESCUnit_Actions::Debris:
			break;
		case ESCUnit_Actions::Construction:
			break;
		case ESCUnit_Actions::ConstructionComplete:
			break;
		case ESCUnit_Actions::BuildUnit:
			break;
		default:
			break;
		}
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

void CSCUnitSpriteComponent::MakePairAction_AnimationName(ESCUnit_Actions ENumAction, const std::string& AnimationName)
{
	auto iter = m_mapUnitActions->find(ENumAction);

	if (iter != m_mapUnitActions->end())
	{
		m_mapUnitActions->erase(iter);
	}

	SCUnit_Anim_LayerNameBind Bind{};
	Bind.AnimName = AnimationName;

	switch (ENumAction)
	{
	case ESCUnit_Actions::Birth:
		Bind.AnimLayer = ESCUnit_AnimLayer::AnimLayer_Shadow_Main;
		break;
	case ESCUnit_Actions::Death:
		Bind.AnimLayer = ESCUnit_AnimLayer::AnimLayer_Shadow_Main;
		break;
	case ESCUnit_Actions::Idle:
		Bind.AnimLayer = ESCUnit_AnimLayer::AnimLayer_Shadow_Main;
		break;
	case ESCUnit_Actions::Move:
		Bind.AnimLayer = ESCUnit_AnimLayer::AnimLayer_Shadow_Main;
		break;
	case ESCUnit_Actions::SurfaceAttack:
		if (m_CBuffer->GetRenderFlags() & ESCUnit_CBufferRenderFlag::Attack)
			Bind.AnimLayer = ESCUnit_AnimLayer::AnimLayer_Attack_Boost;
		else
			Bind.AnimLayer = ESCUnit_AnimLayer::AnimLayer_Shadow_Main;
		break;
	case ESCUnit_Actions::AirAttack:
		if (m_CBuffer->GetRenderFlags() & ESCUnit_CBufferRenderFlag::Attack)
			Bind.AnimLayer = ESCUnit_AnimLayer::AnimLayer_Attack_Boost;
		else
			Bind.AnimLayer = ESCUnit_AnimLayer::AnimLayer_Shadow_Main;
		break;
	case ESCUnit_Actions::Debris:
		Bind.AnimLayer = ESCUnit_AnimLayer::AnimLayer_Shadow_Main;
		break;
	case ESCUnit_Actions::Construction:
		Bind.AnimLayer = ESCUnit_AnimLayer::AnimLayer_Shadow_Main;
		break;
	case ESCUnit_Actions::ConstructionComplete:
		Bind.AnimLayer = ESCUnit_AnimLayer::AnimLayer_Shadow_Main;
		break;
	case ESCUnit_Actions::BuildUnit:
		Bind.AnimLayer = ESCUnit_AnimLayer::AnimLayer_Top;
		break;
	case ESCUnit_Actions::End:
		break;
	default:
		break;
	}

	m_mapUnitActions->insert(std::make_pair(ENumAction, Bind));
}

void CSCUnitSpriteComponent::ChangeAnimationByAction(ESCUnit_Actions ENumAction)
{
	auto iter = m_mapUnitActions->find(ENumAction);
	if (iter == m_mapUnitActions->end())
		return;

	m_AnimationUnitLayer[iter->second.AnimLayer]->ChangeAnimation(iter->second.AnimName);

	//switch (ENumAction)
	//{
	//case ESCUnit_Actions::Birth:
	//	m_AnimationUnitLayer[iter->second.AnimLayer]->ChangeAnimation(iter->second.AnimName);
	//	break;
	//case ESCUnit_Actions::Idle:
	//	m_AnimationUnitLayer[AnimLayer_Shadow_Main]->ChangeAnimation(iter->second);
	//	break;
	//case ESCUnit_Actions::Move:
	//	m_AnimationUnitLayer[AnimLayer_Shadow_Main]->ChangeAnimation(iter->second);
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

void CSCUnitSpriteComponent::CreateNewAnimLayer(ESCUnit_AnimLayer Layer)
{
	if (m_AnimationUnitLayer[Layer] || !m_Material)
		return;

	m_AnimationUnitLayer[Layer] = new CAnimation2D;
	m_AnimationUnitLayer[Layer]->Init();
	m_AnimationUnitLayer[Layer]->SetMaterialTextureInfoPreset((int)Layer + 1);   //�ؽ�ó�� t1���� �����. -> +1

	//���ִ� 2�� ~ 6�� �ؽ�ó���� ����ϵ��� �̸� ���.
	//���߿� ��ӹ޾Ƽ� �ؽ�ó�� �������ָ� ��.
	m_Material->AddTextureEmpty((int)Layer + 1, (int)EShaderBufferType::Pixel);
}

void CSCUnitSpriteComponent::RegisterSequence()
{
	for (auto& iter : *m_mapUnitActions)
	{
		m_AnimationUnitLayer[iter.second.AnimLayer]->AddAnimation(iter.second.AnimName, iter.second.AnimName);
	}
}

CAnimation2D* CSCUnitSpriteComponent::GetUnitAnimLayer(int Index)
{
	if (AnimLayer_Max <= Index)
		return nullptr;

	return m_AnimationUnitLayer[Index];
}

//bool CSCUnitSpriteComponent::AddUnitAnimation(ESCUnit_TextureLayer Layer, const std::string& Name, const std::string& SequenceName, float PlayTime, float PlayScale, bool Loop, bool Reverse)
//{
//	if (SequenceName.empty())
//		return false;
//
//	CAnimationSequence2D* Seq = CResourceManager::GetInst()->FindAnimationSequence2D(Name);
//
//	return AddUnitAnimation(Layer, Name, Seq, PlayTime, PlayScale, Loop, Reverse);
//}

//bool CSCUnitSpriteComponent::AddUnitAnimation(ESCUnit_TextureLayer Layer, const std::string& Name, CAnimationSequence2D* Sequence, float PlayTime, float PlayScale, bool Loop, bool Reverse)
//{
//	if (!Sequence)
//		return false;
//
//	switch (Layer)
//	{
//	case ESCUnit_TextureLayer::UnitMainWithShadow:
//		m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::UseShadowSprite);
//	case ESCUnit_TextureLayer::UnitMainOnly:
//		m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::MainShadow);
//		m_AnimationUnitLayer[AnimLayer_Shadow_Main]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
//		break;
//	case ESCUnit_TextureLayer::Top:
//		m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::MainShadow);
//		m_AnimationUnitLayer[AnimLayer_Top]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
//		break;
//	case ESCUnit_TextureLayer::Effect:
//		m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::MainShadow);
//		m_AnimationUnitLayer[AnimLayer_Top]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
//		break;
//	case ESCUnit_TextureLayer::Attack:
//		m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::MainShadow);
//		m_AnimationUnitLayer[AnimLayer_Top]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
//		break;
//	case ESCUnit_TextureLayer::UnitBoost:
//		m_CBuffer->TurnOnRenderFlags(ESCUnit_CBufferRenderFlag::MainShadow);
//		m_AnimationUnitLayer[AnimLayer_Top]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
//		break;
//	default:
//		break;
//	}
//
//
//	return m_AnimationUnitLayer[(int)Layer]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
//}


