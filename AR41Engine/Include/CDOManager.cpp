#include "CDOManager.h"

#include "GameObject/GameObject.h"

std::unordered_map<std::string, CGameObject*>	m_mapObjectCDO;
std::unordered_map<std::string, CSceneInfo*>	m_mapSceneInfoCDO;
std::unordered_map<std::string, CComponent*>	m_mapComponentCDO;

CCDOManager::CCDOManager()
{
}

CCDOManager::~CCDOManager()
{
}
