#include "SoundManager.h"
#include "Sound.h"

#include "../../Scene/SceneManager.h"

CSoundManager::CSoundManager() :
	m_System(nullptr),
	m_MasterGroup(nullptr)
{
}

CSoundManager::~CSoundManager()
{
	m_mapSound.clear();

	auto	iter = m_mapChannelGroup.begin();
	auto	iterEnd = m_mapChannelGroup.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->release();
	}

	if (m_System)
	{
		m_System->release();
		m_System->close();
	}
}

bool CSoundManager::Init()
{
	// FMOD_RESULT : FMOD���� �����ϴ� Ÿ������ FMOD�� ����� ����Ҷ� �Լ��� �����ߴ��� �����޴�����
	// �Ǵ��ϱ� ���� ���� Ÿ���̴�.
	FMOD_RESULT result = FMOD::System_Create(&m_System);

	if (result != FMOD_OK)
		return false;

	result = m_System->init(500, FMOD_INIT_NORMAL, nullptr);

	if (result != FMOD_OK)
		return false;

	result = m_System->getMasterChannelGroup(&m_MasterGroup);

	if (result != FMOD_OK)
		return false;

	m_mapChannelGroup.insert(std::make_pair("Master", m_MasterGroup));

	CreateSoundChannel("BGM");
	CreateSoundChannel("Effect");
	CreateSoundChannel("UI");

	return true;
}

void CSoundManager::Update()
{
	m_System->update();
}

bool CSoundManager::CreateSoundChannel(const std::string& Name)
{
	if (FindChannelGroup(Name))
		return false;

	FMOD::ChannelGroup* Group = nullptr;

	FMOD_RESULT	result = m_System->createChannelGroup(Name.c_str(), &Group);

	if (result != FMOD_OK)
		return false;

	// ������ �׷��� ������ �׷쿡 �߰����ش�.
	m_MasterGroup->addGroup(Group, false);

	m_mapChannelGroup.insert(std::make_pair(Name, Group));

	return true;
}

CSound* CSoundManager::LoadSound(const std::string& GroupName, const std::string& Name,
	bool Loop, const char* FileName, const std::string& PathName)
{
	CSound* Sound = FindSound(Name);

	if (Sound)
		return Sound;

	FMOD::ChannelGroup* Group = FindChannelGroup(GroupName);

	if (!Group)
		return Sound;

	Sound = new CSound;

	Sound->SetResourceType(EResourceType::Sound);
	Sound->SetName(Name);

	if (!Sound->LoadSound(m_System, Group, Loop, FileName, PathName))
	{
		SAFE_DELETE(Sound);
		return nullptr;
	}

	Sound->SetGroupName(GroupName);

	m_mapSound.insert(std::make_pair(Name, Sound));

	return Sound;
}

bool CSoundManager::SetVolume(int Volume)
{
	m_MasterGroup->setVolume(Volume / 100.f);

	return true;
}

bool CSoundManager::SetVolume(const std::string& GroupName, int Volume)
{
	FMOD::ChannelGroup* Group = FindChannelGroup(GroupName);

	if (!Group)
		return false;

	Group->setVolume(Volume / 100.f);

	return true;
}

bool CSoundManager::SoundPlay(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Play();

	return true;
}

bool CSoundManager::SoundStop(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Stop();

	return true;
}

bool CSoundManager::SoundPause(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Pause();

	return true;
}

bool CSoundManager::SoundResume(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Resume();

	return true;
}

FMOD::ChannelGroup* CSoundManager::FindChannelGroup(const std::string& Name)
{
	auto	iter = m_mapChannelGroup.find(Name);

	if (iter == m_mapChannelGroup.end())
		return nullptr;

	return iter->second;
}

CSound* CSoundManager::FindSound(const std::string& Name)
{
	auto	iter = m_mapSound.find(Name);

	if (iter == m_mapSound.end())
		return nullptr;

	return iter->second;
}

void CSoundManager::ReleaseSound(const std::string& Name)
{
	auto	iter = m_mapSound.find(Name);

	if (iter == m_mapSound.end())
		return;

	if (iter->second->GetRefCount() == 1)
		m_mapSound.erase(iter);
}

void CSoundManager::DeleteUnused()
{
	auto iter = m_mapSound.begin();
	auto iterEnd = m_mapSound.end();

	CSceneManager* SceneMgr = CSceneManager::GetInst();

	while (iter != iterEnd)
	{
		//������ ������ �ʰ� �ʼ� ���ҽ��� �����Ǿ� ���� ���� ��� �����ش�. -> RefCount == 0 �� �ǹǷ� �˾Ƽ� ����
		if (iter->second->GetRefCount() == 1 && !(iter->second->GetEssential()))
		{
			iter = m_mapSound.erase(iter);
			continue;
		}

		SceneMgr->AddSceneResource(iter->second);
		++iter;
	}
}
