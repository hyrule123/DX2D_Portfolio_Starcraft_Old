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
#include "../Component/CameraComponent.h"



CSCUnitSpriteComponent::CSCUnitSpriteComponent():
	m_PrivateSBuffer{}
{
	m_ComponentTypeName = "SCUnitSpriteComponent";
}

CSCUnitSpriteComponent::CSCUnitSpriteComponent(const CSCUnitSpriteComponent& component) :
	CSpriteComponent(component),
	m_SBufferInfo(component.m_SBufferInfo),
	m_PrivateSBuffer(component.m_PrivateSBuffer)
{
	m_Material = component.m_Material->Clone();

	for (int i = 0; i < Anim_Max; ++i)
	{
		m_UnitAnimLayer[i] = component.m_UnitAnimLayer[i]->Clone();
	}
}

CSCUnitSpriteComponent::~CSCUnitSpriteComponent()
{
	for (int i = 0; i < Anim_Max; ++i)
	{
		SAFE_DELETE(m_UnitAnimLayer[i]);
	}
}

bool CSCUnitSpriteComponent::CDOPreload()
{
	if (!CSceneComponent::CDOPreload())
		return false;

	//�޽� ���
	m_Mesh = CResourceManager::GetInst()->FindMesh("CenterUVRect");
	
	m_Transform->Set2D(true);

	//Material ���� �� ���
	CMaterial* Mtrl = CResourceManager::GetInst()->CreateMaterial<CMaterial>("SCUnitMaterial");

	//�ϴ� ���� ������ ����� ����. PrimitiveComponent�� vecMaterial�� ��� ���� ����
	m_vecMaterial.clear();

	//������ ���̴� ����
	Mtrl->SetShader("SCUnitShader");

	//���ִ� 2�� ~ 6�� �ؽ�ó���� ����ϵ��� �̸� ���.
	//���߿� ��ӹ޾Ƽ� �ؽ�ó�� �������ָ� ��.
	Mtrl->AddTextureEmpty(1, (int)EShaderBufferType::Pixel);
	Mtrl->AddTextureEmpty(2, (int)EShaderBufferType::Pixel);
	Mtrl->AddTextureEmpty(3, (int)EShaderBufferType::Pixel);
	Mtrl->AddTextureEmpty(4, (int)EShaderBufferType::Pixel);
	Mtrl->AddTextureEmpty(5, (int)EShaderBufferType::Pixel);



	//���������� �ִϸ��̼��� ����
	for (int i = 0; i < Anim_Max; ++i)
	{
		m_UnitAnimLayer[i] = new CAnimation2D;
		m_UnitAnimLayer[i]->Init();
	}




	

	//�������, ����ȭ���� ����
	m_CBuffer = new CSCUnitConstantBuffer;
	m_CBuffer->Init();

	/*	
	m_SBuffer = new CStructuredBuffer;
	m_SBuffer->Init("SCUnistSBuffer", sizeof(SCUnitSBuffer), 2000, 8, false, (int)EShaderBufferType::Vertex);
	*/

	m_SBufferInfo = new CSharedStructuredBuffer<SCUnitSBuffer>;
	m_SBufferInfo->Init("SCUnitSBuffer", sizeof(SCUnitSBuffer), 400, true, (int)EShaderBufferType::Vertex);


	return true;
}

void CSCUnitSpriteComponent::Start()
{
	CSceneComponent::Start();

	for (int i = 0; i < Anim_Max; ++i)
	{
		m_UnitAnimLayer[i]->Start();
	}

	CRenderManager::GetInst()->AddRenderList(this);
}

bool CSCUnitSpriteComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	for (int i = 0; i < Anim_Max; ++i)
	{
		m_UnitAnimLayer[i]->SetOwner(this);
	}

	return true;
}

void CSCUnitSpriteComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	if (m_Animation)
		m_Animation->Update(DeltaTime);
}

void CSCUnitSpriteComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);


	//�ν��Ͻ��� ������ġ������ �ڽ��� ����ü�� ����
	const Matrix& matWorld = m_Transform->GetWorldMatrix();
	const Matrix& matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	const Matrix& matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();

	//����-��-������� ���
	m_PrivateSBuffer.matWVP = matWorld * matView * matProj;
	m_PrivateSBuffer.matWVP.Transpose();

	m_PrivateSBuffer.Pivot = m_Transform->GetPivot();
	m_PrivateSBuffer.MeshSize = m_Transform->GetMeshSize();

	//�����÷��� ���(�ӽ�)
	

}

void CSCUnitSpriteComponent::Render()
{
		//CSceneComponent::Render();
	
		//if (m_CurAnimation.empty() || !m_CurAnimation->m_vecSequence[0] ||
		//	!m_CurAnimation->m_vecSequence[0]->GetTexture())
		//	return;
	
		//CAnimation2DConstantBuffer* Buffer = CResourceManager::GetInst()->GetAnim2DConstantBuffer();
		
		
		EAnimation2DType	Type = m_CurAnimation->m_vecSequence[0]->GetAnim2DType();
	
		if (Type == EAnimation2DType::Atlas)
		{
			const Animation2DFrameData& FrameData = m_CurAnimation->m_vecSequence[0]->GetFrameData(m_CurAnimation->m_Frame);
	
			Buffer->SetImageSize((float)m_CurAnimation->m_vecSequence[0]->GetTexture()->GetWidth(),
				(float)m_CurAnimation->m_vecSequence[0]->GetTexture()->GetHeight());
			Buffer->SetImageFrame(FrameData.Start, FrameData.End);
		}
		else if (Type == EAnimation2DType::AtlasIndexed)
		{
			int CalcedFrame = (m_CurAnimation->m_Frame) * (m_CurAnimation->m_vecSequence[0]->GetRowNum());
	
			//int Direction = (int)m_Owner->GetRowIndex();
	
	
			if (Direction > 16)
			{
				Direction = 31 - Direction;
				Buffer->SetXFlip(true);
			}
			else
				Buffer->SetXFlip(false);
	
			CalcedFrame += Direction;
	
	
			const Animation2DFrameData& FrameData = m_CurAnimation->m_vecSequence[0]->GetFrameData(CalcedFrame);
	
			Buffer->SetImageSize((float)m_CurAnimation->m_vecSequence[0]->GetTexture()->GetWidth(),
				(float)m_CurAnimation->m_vecSequence[0]->GetTexture()->GetHeight());
			Buffer->SetImageFrame(FrameData.Start, FrameData.End);
		}
	
		else if (Type == EAnimation2DType::Frame)
		{
			if (m_Owner)
			{
				m_Owner->SetTexture(m_CurAnimation->m_vecSequence[0]->GetTexture());
				m_Owner->SetTextureFrameIndex(m_CurAnimation->m_Frame);
			}
	
			else
			{
				m_CurAnimation->m_vecSequence[0]->GetTexture()->SetShader(0,
					(int)EShaderBufferType::Pixel, m_CurAnimation->m_Frame);
			}
		}
	
		else
		{
			Buffer->SetFrame(m_CurAnimation->m_Frame);
		}
	
		Buffer->SetImageType(m_CurAnimation->m_vecSequence[0]->GetAnim2DType());
		Buffer->SetAnim2DEnable(true);
	
		Buffer->UpdateBuffer();
	
	
		
	
	
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

void CSCUnitSpriteComponent::RenderInstanced()
{
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

CAnimation2D* CSCUnitSpriteComponent::GetUnitAnimLayer(int Index)
{
	if (Anim_Max <= Index)
		return nullptr;

	return m_UnitAnimLayer[Index];
}

bool CSCUnitSpriteComponent::AddUnitAnimation(ETextureLayer Layer, const std::string& Name, const std::string& SequenceName, float PlayTime, float PlayScale, bool Loop, bool Reverse)
{
	CAnimationSequence2D* Seq = CResourceManager::GetInst()->FindAnimationSequence2D(Name);

	return AddUnitAnimation(Layer, Name, Seq, PlayTime, PlayScale, Loop, Reverse);
}

bool CSCUnitSpriteComponent::AddUnitAnimation(ETextureLayer Layer, const std::string& Name, CAnimationSequence2D* Sequence, float PlayTime, float PlayScale, bool Loop, bool Reverse)
{
	if (!Sequence)
		return false;

	switch (Layer)
	{
	case ETextureLayer::UnitShadow:
		m_CBuffer->TurnOnRenderFlags(ERenderFlag::UseShadowSprite);
	case ETextureLayer::UnitMain:
		m_CBuffer->TurnOnRenderFlags(ERenderFlag::UnitMainShadow);
		m_UnitAnimLayer[Anim_Shadow_Main]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
		break;
	case ETextureLayer::UnitTop:
		m_CBuffer->TurnOnRenderFlags(ERenderFlag::UnitMainShadow);
		m_UnitAnimLayer[Anim_Top]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
		break;
	case ETextureLayer::UnitEffect:
		m_CBuffer->TurnOnRenderFlags(ERenderFlag::UnitMainShadow);
		m_UnitAnimLayer[Anim_Top]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
		break;
	case ETextureLayer::UnitAttack:
		m_CBuffer->TurnOnRenderFlags(ERenderFlag::UnitMainShadow);
		m_UnitAnimLayer[Anim_Top]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
		break;
	case ETextureLayer::UnitBoost:
		m_CBuffer->TurnOnRenderFlags(ERenderFlag::UnitMainShadow);
		m_UnitAnimLayer[Anim_Top]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
		break;
	default:
		break;
	}


	return m_UnitAnimLayer[(int)Layer]->AddAnimation(Name, Sequence, PlayTime, PlayScale, Loop, Reverse);
}


