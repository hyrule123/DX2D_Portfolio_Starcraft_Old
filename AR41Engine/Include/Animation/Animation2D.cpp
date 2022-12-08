#include "Animation2D.h"
#include "../Component/SpriteComponent.h"
#include "../Scene/Scene.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"


CAnimation2D::CAnimation2D() :
	m_Owner(nullptr),
	m_Play(true),
	m_UseInstancing(false)
{
	m_ClassName = "Animation2D";
}

CAnimation2D::CAnimation2D(const CAnimation2D& Anim) :
	CRef(Anim),
	m_Play(Anim.m_Play),
	m_UseInstancing(Anim.m_UseInstancing)
{
	m_mapAnimation.clear();

	auto	iter = Anim.m_mapAnimation.begin();
	auto	iterEnd = Anim.m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		CAnimation2DData* Data = iter->second->Clone();

		if (Data->GetName() == Anim.m_CurAnimation->GetName())
		{
			m_CurAnimation = Data;
		}

		m_mapAnimation.insert(std::make_pair(iter->first, Data));
	}
}

CAnimation2D::~CAnimation2D()
{
	auto	iter = m_mapAnimation.begin();
	auto	iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

void CAnimation2D::SetOwner(CSpriteComponent* Owner)
{
	m_Owner = Owner;
}

CScene* CAnimation2D::GetScene() const
{
	if (!m_Owner)
		return nullptr;

	return m_Owner->GetScene();
}

void CAnimation2D::Start()
{
	if (m_Owner && m_CurAnimation)
	{
		m_Owner->SetTexture(m_CurAnimation->GetTexture());
	}
}

bool CAnimation2D::Init()
{
	return true;
}

void CAnimation2D::Update(float DeltaTime)
{
	if (!m_Play || !m_CurAnimation ||
		m_CurAnimation->GetFrameCount() == 0)
		return;

	m_CurAnimation->Update(DeltaTime);
}

bool CAnimation2D::AddAnimation(const std::string& Name,
	const std::string& SequenceName, float PlayTime, float PlayScale,
	EAnimLoopMethod LoopMethod, bool Reverse)
{
	CAnimation2DData* Anim = FindAnimation(Name);

	if (Anim)
		return true;

	CAnimationSequence2D* Seq = CResourceManager::GetInst()->FindAnimationSequence2D(SequenceName);
	if (!Seq)
		return false;

	Anim = new CAnimation2DData;
	Anim->SetSequence(Seq, Name, SequenceName, PlayTime, PlayScale, LoopMethod, Reverse);


	if (m_mapAnimation.empty())
	{
		m_CurAnimation = Anim;

		if (m_Owner)
		{
			m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
		}
	}

	m_mapAnimation.insert(std::make_pair(Name, Anim));

	return true;
}

bool CAnimation2D::AddAnimation(const std::string& Name,
	CAnimationSequence2D* Sequence,
	float PlayTime, float PlayScale, EAnimLoopMethod LoopMethod, bool Reverse)
{
	CAnimation2DData* Anim = FindAnimation(Name);

	if (Anim || !Sequence)
		return true;

	const std::string& SeqName = Sequence->GetName();

	Anim = new CAnimation2DData;
	Anim->SetSequence(Sequence, Name, SeqName, PlayTime, PlayScale, LoopMethod, Reverse);


	if (m_mapAnimation.empty())
	{
		m_CurAnimation = Anim;

		if (m_Owner)
		{
			m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
		}
	}

	m_mapAnimation.insert(std::make_pair(Name, Anim));

	return true;
}



void CAnimation2D::SetPlayTime(const std::string& Name, float PlayTime)
{
	CAnimation2DData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->m_PlayTime = PlayTime;
}

void CAnimation2D::SetPlayScale(const std::string& Name, float PlayScale)
{
	CAnimation2DData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->m_PlayScale = PlayScale;
}

void CAnimation2D::SetLoop(const std::string& Name, EAnimLoopMethod LoopMethod)
{
	CAnimation2DData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->SetLoopMethod(LoopMethod);
}

void CAnimation2D::SetReverse(const std::string& Name, bool Reverse)
{
	CAnimation2DData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->m_Reverse = Reverse;
}

void CAnimation2D::SetCurrentAnimation(const std::string& Name)
{
	CAnimation2DData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	m_CurAnimation->SetInitialValue();

	m_CurAnimation = Anim;

	if (m_Owner)
	{
		m_Owner->SetTexture(m_CurAnimation->GetTexture());
		m_Owner->SetTextureFrameIndex(0);
	}
}

void CAnimation2D::ChangeAnimation(const std::string& Name)
{
	if (nullptr != m_CurAnimation && m_CurAnimation->GetName() == Name)
		return;

	m_CurAnimation->SetInitialValue();

	m_CurAnimation = FindAnimation(Name);

	m_CurAnimation->m_Frame = 0;
	m_CurAnimation->m_Time = 0.f;

	if (m_Owner && false == m_UseInstancing)
	{
		m_Owner->SetTexture(m_CurAnimation->GetTexture(), m_CurAnimation->GetMaterialTextureInfoPreset());
		m_Owner->SetTextureFrameIndex(0);
	}
}

inline void CAnimation2D::ChangeTexture(CTexture* Tex)
{
	if (!m_Owner)
		return;

	m_Owner->SetTexture(Tex, 0);
}

void CAnimation2D::Save(FILE* File)
{
	CRef::Save(File);

	int	Count = (int)m_mapAnimation.size();

	fwrite(&Count, 4, 1, File);

	auto	iter = m_mapAnimation.begin();
	auto	iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->Save(File);
	}

	int	Length = (int)m_CurAnimation->GetName().length();

	fwrite(&Length, 4, 1, File);
	fwrite(m_CurAnimation->GetName().c_str(), 1, Length, File);
}

void CAnimation2D::Load(FILE* File)
{
	CRef::Load(File);

	auto	iter = m_mapAnimation.begin();
	auto	iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	m_mapAnimation.clear();

	m_CurAnimation = nullptr;


	int	Count = 0;

	fread(&Count, 4, 1, File);

	for (int i = 0; i < Count; ++i)
	{
		CAnimation2DData* Data = new CAnimation2DData;

		Data->m_Owner = this;
		Data->Load(File);

		m_mapAnimation.insert(std::make_pair(Data->GetName(), Data));
	}

	int	Length = 0;
	char	CurName[256] = {};

	fread(&Length, 4, 1, File);
	fread(CurName, 1, Length, File);

	m_CurAnimation = FindAnimation(CurName);
}

CAnimation2D* CAnimation2D::Clone()
{
	return new CAnimation2D(*this);
}

void CAnimation2D::SetShader()
{
	if (!m_CurAnimation || !m_CurAnimation->m_Sequence ||
		!m_CurAnimation->m_Sequence->GetTexture())
		return;

	CAnimation2DConstantBuffer* Buffer = CResourceManager::GetInst()->GetAnim2DConstantBuffer();


	EAnimation2DType	Type = m_CurAnimation->m_Sequence->GetAnim2DType();

	if (Type == EAnimation2DType::Atlas)
	{
		const Animation2DFrameData* FrameData = m_CurAnimation->m_Sequence->GetFrameData(m_CurAnimation->m_Frame);

		Buffer->SetImageSize((float)m_CurAnimation->m_Sequence->GetTexture()->GetWidth(),
			(float)m_CurAnimation->m_Sequence->GetTexture()->GetHeight());
		Buffer->SetImageFrame(FrameData->Start, FrameData->End);
	}
	else if (Type == EAnimation2DType::AtlasIndexed)
	{
		int CalcedFrame = (m_CurAnimation->m_Frame) * (m_CurAnimation->GetAnimationSequence()->GetRowSize());

		int Direction = (int)m_Owner->GetRowIndex();


		if (Direction > 16)
		{
			Direction = 31 - Direction;
			Buffer->SetXFlip(true);
		}
		else
			Buffer->SetXFlip(false);

		CalcedFrame += Direction;


		const Animation2DFrameData* FrameData = m_CurAnimation->m_Sequence->GetFrameData(CalcedFrame);

		Buffer->SetImageSize((float)m_CurAnimation->m_Sequence->GetTexture()->GetWidth(),
			(float)m_CurAnimation->m_Sequence->GetTexture()->GetHeight());
		Buffer->SetImageFrame(FrameData->Start, FrameData->End);
	}

	else if (Type == EAnimation2DType::Frame)
	{
		if (m_Owner)
		{
			m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
			m_Owner->SetTextureFrameIndex(m_CurAnimation->m_Frame);
		}

		else
		{
			m_CurAnimation->m_Sequence->GetTexture()->SetShader(0,
				(int)EShaderBufferType::Pixel, m_CurAnimation->m_Frame);
		}
	}

	else
	{
		Buffer->SetFrame(m_CurAnimation->m_Frame);
	}

	Buffer->SetImageType(m_CurAnimation->m_Sequence->GetAnim2DType());
	Buffer->SetAnim2DEnable(true);

	Buffer->UpdateBuffer();
}

CAnimation2DData* CAnimation2D::FindAnimation(const std::string& Name)
{
	auto	iter = m_mapAnimation.find(Name);

	if (iter == m_mapAnimation.end())
		return nullptr;

	return iter->second;
}


std::string CAnimation2D::GetCurrentAnimationName() const
{
	if (!m_CurAnimation)
		return "";

	return m_CurAnimation->GetName();
}

int CAnimation2D::GetCurrentFrame() const
{
	if (!m_CurAnimation)
		return -1;

	return m_CurAnimation->GetCurrentFrame();
}

float CAnimation2D::GetCurrentAnimationTime() const
{
	if (!m_CurAnimation)
		return -1.f;

	return m_CurAnimation->GetAnimationTime();
}

CAnimationSequence2D* CAnimation2D::GetCurrentAnimationSequence() const
{
	if (!m_CurAnimation)
		return nullptr;

	return m_CurAnimation->GetAnimationSequence();
}

const Animation2DFrameData* CAnimation2D::GetCurrentAnimationFrameData() const
{
	if(!m_CurAnimation)
		return nullptr;

	return m_CurAnimation->GetCurrentAnimationFrameData();
}

const Animation2DFrameData* CAnimation2D::GetCurrentAnimationFrameDataSCUnit(int Dir) const
{
	if (!m_CurAnimation)
		return nullptr;

	return m_CurAnimation->GetSCUnitFrameData(Dir);
}

CAnimation2DData* CAnimation2D::GetCurrentAnimation() const
{
	return m_CurAnimation;
}

int CAnimation2D::GetCurAnimMaterialTextureInfoPreset() const
{
	if (!m_CurAnimation)
		return -1;

	return m_CurAnimation->GetMaterialTextureInfoPreset();
}

