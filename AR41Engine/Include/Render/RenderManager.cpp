
#include "RenderManager.h"
#include "../Component/SceneComponent.h"
#include "RenderState.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneViewport.h"
#include "BlendState.h"
#include "DepthStencilState.h"

#include "../CDO.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()	:
	m_RenderStateManager(nullptr)
{
}

CRenderManager::~CRenderManager()
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	SAFE_DELETE(m_RenderStateManager);
}

void CRenderManager::CreateLayer(const std::string& Name, int Priority)
{
	RenderLayer* Layer = new RenderLayer;

	Layer->Name = Name;
	Layer->LayerPriority = Priority;

	m_RenderLayerList.push_back(Layer);

	std::sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::SortLayer);
}

bool CRenderManager::Init()
{
	m_RenderStateManager = new CRenderStateManager;

	if (!m_RenderStateManager->Init())
		return false;

	CreateLayer("Default", 1);
	CreateLayer("Back", 0);

	SetLayerAlphaBlend("Default");

	m_AlphaBlend = m_RenderStateManager->FindRenderState<CBlendState>("AlphaBlend");
	m_DepthDisable = m_RenderStateManager->FindRenderState<CDepthStencilState>("DepthDisable");

	return true;
}

void CRenderManager::Render(float DeltaTime)
{
	{
		size_t NumLayer = m_RenderLayerList.size();


		for (size_t i = 0; i < NumLayer; ++i)
		{
			auto	iterRenderList = m_RenderLayerList[i]->RenderList.begin();
			auto	iterRenderListEnd = m_RenderLayerList[i]->RenderList.end();

			if (m_RenderLayerList[i]->AlphaBlend)
				m_RenderLayerList[i]->AlphaBlend->SetState();


			//Render ȣ��
			for (; iterRenderList != iterRenderListEnd;)
			{
				if (!(*iterRenderList)->GetActive())
				{
					iterRenderList = m_RenderLayerList[i]->RenderList.erase(iterRenderList);
					iterRenderListEnd = m_RenderLayerList[i]->RenderList.end();
					continue;
				}

				else if (!(*iterRenderList)->GetEnable())
				{
					++iterRenderList;
					continue;
				}

				(*iterRenderList)->Render();
				++iterRenderList;
			}


			for (auto& iter : m_RenderLayerList[i]->RenderInstancingMap)
			{
				iter.second->RenderInstanced();
			}



			if (m_RenderLayerList[i]->AlphaBlend)
				m_RenderLayerList[i]->AlphaBlend->ResetState();
		}
	}



	// 2D, 3D ��ü�� ��� ����ߴٸ� UI�� ������ش�.
	// ���̹��۸� �Ⱦ��� ���ĺ��带 �����Ѵ�.
	m_AlphaBlend->SetState();
	m_DepthDisable->SetState();

	CSceneManager::GetInst()->GetScene()->GetViewport()->Render();

	m_DepthDisable->ResetState();
	m_AlphaBlend->ResetState();
}



void CRenderManager::SetLayerPriority(const std::string& Name, int Priority)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Name)
		{
			(*iter)->LayerPriority = Priority;
			break;
		}
	}

	std::sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::SortLayer);
}

void CRenderManager::SetLayerAlphaBlend(const std::string& Name)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Name)
		{
			(*iter)->AlphaBlend = FindRenderState<CRenderState>("AlphaBlend");
			break;
		}
	}
}

void CRenderManager::DeleteLayer(const std::string& Name)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Name)
		{
			SAFE_DELETE((*iter));
			iter = m_RenderLayerList.erase(iter);
			break;
		}
	}
}

void CRenderManager::AddRenderList(CSceneComponent* Component, bool UseInstancing)
{
	size_t size = m_RenderLayerList.size();

	const std::string& RenderLayerName = Component->GetRenderLayerName();

	for (size_t i = 0; i < size; ++i)
	{
		if (m_RenderLayerList[i]->Name == RenderLayerName)
		{
			m_RenderLayerList[i]->RenderList.push_back(Component);

			//�ν��Ͻ��� ����ϰڴٰ� ���޹�����
			if (UseInstancing)
			{
				//�̸��� �޾Ƽ�
				const std::string& TypeName = Component->GetTypeName();

				//�ν��Ͻ� �ʿ� �̹� ��ϵǾ� �ִ��� Ȯ��. �̹� ��ϵǾ� ���� ���� return
				if (m_RenderLayerList[i]->RenderInstancingMap.find(TypeName) != m_RenderLayerList[i]->RenderInstancingMap.end())
					return;

				//�߰��Ǿ����� ������ �ν��Ͻ� �ʿ� �߰��Ѵ�.
				//�ν��Ͻ��� ��� �� �ϳ��� Ŭ���� �ν��Ͻ� �ּҸ� ������������ ��.
				//PLO ����Ʈ�� ����� �ּҸ� ����ϸ� �ɵ�.(PLO�� ���� ����Ǳ� �������� �����ǹǷ�)
				CSceneComponent* PLO = static_cast<CSceneComponent*>(CCDO::FindPLO(TypeName));
				m_RenderLayerList[i]->RenderInstancingMap.insert(std::make_pair(Component->GetTypeName(), PLO));
			}
			

			break;
		}
	}
}

void CRenderManager::AddInstancingMap(CSceneComponent* Component)
{
	size_t size = m_RenderLayerList.size();

	const std::string& RenderLayerName = Component->GetRenderLayerName();

	for (size_t i = 0; i < size; ++i)
	{
		if (m_RenderLayerList[i]->Name == RenderLayerName)
		{
			//�̸��� �޾Ƽ�
			const std::string& TypeName = Component->GetTypeName();

			//�ν��Ͻ� �ʿ� �̹� ��ϵǾ� �ִ��� Ȯ��. �̹� ��ϵǾ� ���� ���� return
			if (m_RenderLayerList[i]->RenderInstancingMap.find(TypeName) != m_RenderLayerList[i]->RenderInstancingMap.end())
				return;

			//�߰��Ǿ����� ������ �ν��Ͻ� �ʿ� �߰��Ѵ�.
			//�ν��Ͻ��� ��� �� �ϳ��� Ŭ���� �ν��Ͻ� �ּҸ� ������������ ��.
			//PLO ����Ʈ�� ����� �ּҸ� ����ϸ� �ɵ�.(PLO�� ���� ����Ǳ� �������� �����ǹǷ�)
			CSceneComponent* PLO = static_cast<CSceneComponent*>(CCDO::FindPLO(TypeName));
			m_RenderLayerList[i]->RenderInstancingMap.insert(std::make_pair(Component->GetTypeName(), PLO));
			break;
		}
	}
}



void CRenderManager::SetBlendFactor(const std::string& Name, float r, float g, float b, float a)
{
	m_RenderStateManager->SetBlendFactor(Name, r, g, b, a);
}

void CRenderManager::AddBlendInfo(const std::string& Name, bool BlendEnable, 
	D3D11_BLEND SrcBlend, D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp,
	D3D11_BLEND SrcAlphBlend, D3D11_BLEND DestAlphBlend, D3D11_BLEND_OP BlendAlphOp,
	UINT8 WriteMask)
{
	m_RenderStateManager->AddBlendInfo(Name, BlendEnable, SrcBlend, DestBlend, BlendOp,
		SrcAlphBlend, DestAlphBlend, BlendAlphOp, WriteMask);
}

bool CRenderManager::CreateBlendState(const std::string& Name,
	bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	return m_RenderStateManager->CreateBlendState(Name, AlphaToCoverageEnable, IndependentBlendEnable);
}

bool CRenderManager::CreateDepthStencil(const std::string& Name, 
	bool DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteMask, 
	D3D11_COMPARISON_FUNC DepthFunc, bool StencilEnable, 
	UINT8 StencilReadMask, UINT8 StencilWriteMask, 
	D3D11_DEPTH_STENCILOP_DESC FrontFace, D3D11_DEPTH_STENCILOP_DESC BackFace)
{
	return m_RenderStateManager->CreateDepthStencil(Name, DepthEnable,
		DepthWriteMask, DepthFunc, StencilEnable, StencilReadMask, StencilWriteMask,
		FrontFace, BackFace);
}

bool CRenderManager::SortLayer(RenderLayer* Src, RenderLayer* Dest)
{
	return Src->LayerPriority < Dest->LayerPriority;
}
