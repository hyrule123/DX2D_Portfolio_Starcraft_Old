#include "SCUltraliskComponent.h"

#include "Resource/Material/Material.h"
#include "Resource/ResourceManager.h"
#include "Resource/Shader/Animation2DConstantBuffer.h"
#include "Component/SCUnitRootComponent.h"

CSCUltraliskComponent::CSCUltraliskComponent() 
{
	m_ComponentTypeName = "UnitSpriteComponent";
}

CSCUltraliskComponent::CSCUltraliskComponent(const CSCUltraliskComponent& component) :
	CSCUnitSpriteComponent(component)
{

}

CSCUltraliskComponent::~CSCUltraliskComponent()
{
}




void CSCUltraliskComponent::Start()
{
	CSCUnitSpriteComponent::Start();

	if (!m_Parent || !m_Parent->CheckTypeID<CSCUnitRootComponent>())
		assert(0);

	m_SCUnitRoot = (CSCUnitRootComponent*)m_Parent;
}

bool CSCUltraliskComponent::CDOPreload()
{
	//������ �ϵ��ڵ������� �ۼ��ϰ� ���߿� ������������ �о�ͼ� SCUnitManager�κ��� �޾Ƽ� ����ؾ���
	//(���ϸ� �ش��ϴ� SCUnitInfo�� ��ϵǾ� ������ �ڵ����� �о�� ����
	//-> CSCSpriteComponent::SetSCUnitInfo()	
	m_SCUnitInfo = new SCUnitInfo;
	//memset(m_SCUnitInfo, 0, sizeof(SCUnitInfo));
	strcpy_s(m_SCUnitInfo->UnitName, SCUnitNameLenMax, "Ultralisk");
	

	//��Ʈ�󸮽�ũ ��������Ʈ �ε�
	//�ؽ�ó �ε��� csv�� �ε��ؼ� �Ϸ�Ǿ��� ���̹Ƿ� �ܼ��� �ε�� �ؽ�ó�� ã�⸸ �ϸ� ��
	CTexture* UltraTex = CResourceManager::GetInst()->FindTexture("UltraLisk");

	if (!UltraTex)
		assert(0);

	
	CResourceManager::GetInst()->CreateAnimationSequence2D(
		"UltraIdle", UltraTex);

	//CResourceManager::GetInst()->SetAnimationSequence2DFrameAllByTileNumber("UltraIdle", 17, 16);

	CResourceManager::GetInst()->AddAnimationSequence2DFrameByTileNumber("UltraIdle", EAnimation2DType::AtlasIndexed, 16, 17, 0, 1);
	strcpy_s(m_SCUnitInfo->AnimationNames[(int)ESCUnit_Actions::Idle], SCUnitNameLenMax, "UltraIdle");
	m_SCUnitInfo->UsingTextureLayerFlags = (UINT8)ESCUnit_TextureLayerFlag::Main;

	CResourceManager::GetInst()->CreateAnimationSequence2D(
		"UltraMove", UltraTex);
	CResourceManager::GetInst()->AddAnimationSequence2DFrameByTileNumber("UltraMove", EAnimation2DType::AtlasIndexed, 16, 17, 1, 9);
	strcpy_s(m_SCUnitInfo->AnimationNames[(int)ESCUnit_Actions::Move], SCUnitNameLenMax, "UltraMove");

	CResourceManager::GetInst()->CreateAnimationSequence2D(
		"UltraAttack", UltraTex);
	CResourceManager::GetInst()->AddAnimationSequence2DFrameByTileNumber("UltraAttack", EAnimation2DType::AtlasIndexed, 16, 17, 10, 15);
	strcpy_s(m_SCUnitInfo->AnimationNames[(int)ESCUnit_Actions::SurfaceAttack], SCUnitNameLenMax, "UltraAttack");

	CSCUnitSpriteComponent::CDOPreload();

	if(m_MaterialSCUnit)
		m_MaterialSCUnit->SetColorKeyUnsignedChar(0, 0, 0);

	return true;
}

bool CSCUltraliskComponent::Init()
{
	if (!CSCUnitSpriteComponent::Init())
		return false;


	//MakePair

	return true;
}

void CSCUltraliskComponent::Update(float DeltaTime)
{
	CSCUnitSpriteComponent::Update(DeltaTime);
}

void CSCUltraliskComponent::PostUpdate(float DeltaTime)
{
	CSCUnitSpriteComponent::PostUpdate(DeltaTime);
}

void CSCUltraliskComponent::Render()
{
	CSCUnitSpriteComponent::Render();
}

CSCUltraliskComponent* CSCUltraliskComponent::Clone() const
{
	return new CSCUltraliskComponent(*this);
}

void CSCUltraliskComponent::Save(FILE* File)
{
	CSCUnitSpriteComponent::Save(File);
}

void CSCUltraliskComponent::Load(FILE* File)
{
	CSCUnitSpriteComponent::Load(File);
}

int CSCUltraliskComponent::GetRowIndex() const
{
	return m_SCUnitRoot->GetDirection();
}


