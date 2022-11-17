#include "GameResource.h"
#include "../Scene/Scene.h"

CGameResource::CGameResource() :
	m_ResourceType(),
	m_Essential(false)
{
}

CGameResource::CGameResource(const CGameResource& Res):
	CRef(Res),
	m_ResourceType(Res.m_ResourceType),
	m_Essential(Res.m_Essential)
{
}

CGameResource::~CGameResource()
{
}


void CGameResource::SetScene(CScene* Scene)
{
	m_Scene = Scene;
}