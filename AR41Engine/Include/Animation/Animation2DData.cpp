
#include "Animation2DData.h"
#include "Animation2D.h"
#include "../Scene/Scene.h"

#include "../Resource/ResourceManager.h"

CAnimation2DData::CAnimation2DData() :
	m_Frame(0),
	m_Time(0.f),
	m_FrameTime(0.f),
	m_PlayTime(0.f),
	m_PlayScale(0.f),
	m_LoopMethod(EAnimLoopMethod::NoLoop),
	m_Reverse(false),
	m_EndFunctionCalled(false),
	m_MaterialTextureInfoIndexPreset(0)
{
}

CAnimation2DData::CAnimation2DData(const CAnimation2DData& Anim):
	m_Name(Anim.m_Name),
	m_SequenceName(Anim.m_SequenceName),
	m_Sequence(Anim.m_Sequence),
	m_Frame(Anim.m_Frame),
	m_Time(Anim.m_Time),
	m_FrameTime(Anim.m_FrameTime),
	m_PlayTime(Anim.m_PlayTime),
	m_PlayScale(Anim.m_PlayScale),
	m_LoopMethod(Anim.m_LoopMethod),
	m_Reverse(Anim.m_Reverse),
	m_EndFunctionCalled(Anim.m_EndFunctionCalled),
	m_SeriesAnimName(Anim.m_SeriesAnimName),
	m_MaterialTextureInfoIndexPreset(Anim.m_MaterialTextureInfoIndexPreset)
{
}

CAnimation2DData::~CAnimation2DData()
{
	for (auto& iter : m_mapNotify)
	{
		SAFE_DELETE(iter.second);
	}
}

void CAnimation2DData::Update(float DeltaTime)
{
	m_Time += DeltaTime * m_PlayScale;

	bool	AnimEnd = false;

	m_FrameTime = m_PlayTime / m_Sequence->GetFrameCount();

	if (m_Time >= m_FrameTime)
	{
		m_Time -= m_FrameTime;

		if (m_Reverse)
		{
			--m_Frame;

			if (m_Frame < 0)
			{
				m_Frame = 0;
				AnimEnd = true;
			}
				
		}

		else
		{
			++m_Frame;

			if (m_Frame == m_Sequence->GetFrameCount())
			{
				m_Frame = m_Sequence->GetFrameCount();
				AnimEnd = true;
			}
				
		}
	}

	auto iter = m_mapNotify.find(m_Frame);
	if (iter != m_mapNotify.end() && !(iter->second->Call))
	{
		size_t size = iter->second->vecFunction.size();
		for (size_t i = 0; i < size; ++i)
		{
			iter->second->vecFunction[i]();
		}
		iter->second->Call = true;
	}

	if (AnimEnd)
	{
		if (m_EndFunction && !m_EndFunctionCalled)
		{
			m_EndFunctionCalled = true;
			m_EndFunction();
		}

		switch (m_LoopMethod)
		{
		case EAnimLoopMethod::NoLoop:
			//아무것도 안해도 됨
			break;
		case EAnimLoopMethod::NormalLoop:
		{
			if (m_Reverse)
				m_Frame = m_Sequence->GetFrameCount() - 1;

			else
				m_Frame = 0;

			for (auto& iter : m_mapNotify)
				iter.second->Call = false;

			m_EndFunctionCalled = false;
			break;
		}
		case EAnimLoopMethod::LoopZigZag:
		{
			m_Reverse = !m_Reverse;

			for (auto& iter : m_mapNotify)
				iter.second->Call = false;

			m_EndFunctionCalled = false;
			break;
		}
		default:
			break;
		}


		if (!m_SeriesAnimName.empty())
			m_Owner->ChangeAnimation(m_SeriesAnimName);
	}

}

void CAnimation2DData::SetSequence(CAnimationSequence2D* Seq, const std::string& Name,
	const std::string& SequenceName, float PlayTime, float PlayScale,
	EAnimLoopMethod LoopMethod, bool Reverse)
{
	m_Sequence = Seq;
	m_Name = Name;
	m_SequenceName = SequenceName;
	m_PlayTime = PlayTime;
	m_PlayScale = PlayScale;
	m_LoopMethod = LoopMethod;
	m_Reverse = Reverse;
	m_FrameCount = m_Sequence->GetFrameCount();
}

void CAnimation2DData::Save(FILE* File)
{
	int	Length = (int)m_Name.length();
	fwrite(&Length, 4, 1, File);
	fwrite(m_Name.c_str(), 1, Length, File);

	Length = (int)m_SequenceName.length();
	fwrite(&Length, 4, 1, File);
	fwrite(m_SequenceName.c_str(), 1, Length, File);

	fwrite(&m_Frame, 4, 1, File);
	fwrite(&m_Time, 4, 1, File);
	fwrite(&m_FrameTime, 4, 1, File);
	fwrite(&m_PlayTime, 4, 1, File);
	fwrite(&m_PlayScale, 4, 1, File);

	fwrite(&m_LoopMethod, 1, 1, File);
	fwrite(&m_Reverse, 1, 1, File);

	size_t SeriesAnimLen = m_SeriesAnimName.length();
	fwrite(&SeriesAnimLen, sizeof(size_t), 1, File);
	fwrite(m_SeriesAnimName.c_str(), 1, SeriesAnimLen, File);
}

void CAnimation2DData::Load(FILE* File)
{
	int	Length = 0;
	char	Name[256] = {};
	fread(&Length, 4, 1, File);
	fread(Name, 1, Length, File);

	m_Name = Name;

	Length = 0;
	char	SequenceName[256] = {};
	fread(&Length, 4, 1, File);
	fread(SequenceName, 1, Length, File);

	m_SequenceName = SequenceName;

	fread(&m_Frame, 4, 1, File);
	fread(&m_Time, 4, 1, File);
	fread(&m_FrameTime, 4, 1, File);
	fread(&m_PlayTime, 4, 1, File);
	fread(&m_PlayScale, 4, 1, File);

	fread(&m_LoopMethod, 1, 1, File);
	fread(&m_Reverse, 1, 1, File);

	m_Sequence = CResourceManager::GetInst()->FindAnimationSequence2D(SequenceName);

	size_t SeriesAnimLen = m_SeriesAnimName.length();
	fread(&SeriesAnimLen, sizeof(size_t), 1, File);

	char str[MAX_PATH] = {};
	fread(str, 1, SeriesAnimLen, File);
	m_SeriesAnimName = str;
}

CAnimation2DData* CAnimation2DData::Clone()
{
	return new CAnimation2DData(*this);
}

inline void CAnimation2DData::SetInitialValue()
{
	m_Time = 0.f;

	if (m_Reverse)
		m_Frame = m_Sequence->GetFrameCount() - 1;
	else
		m_Frame = 0;

	for (auto& iter : m_mapNotify)
	{
		iter.second->Call = false;
	}

	m_EndFunctionCalled = false;
}

CTexture* CAnimation2DData::GetTexture() const
{
	if (!m_Sequence)
		return nullptr;

	return m_Sequence->GetTexture();
}
