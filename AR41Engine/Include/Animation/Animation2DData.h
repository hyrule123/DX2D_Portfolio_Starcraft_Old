#pragma once

#include "../EngineInfo.h"
#include "../Resource/Animation/AnimationSequence2D.h"

struct Animation2DNotify
{
	std::string	Name;
	int		Frame;
	float	Time;
	bool	Call;
	std::vector<std::function<void()>>	vecFunction;

	Animation2DNotify() :
		Frame(0),
		Time(0.f),
		Call(false)
	{
	}
};



class CAnimation2DData
{
	friend class CAnimation2D;

private:
	CAnimation2DData();
	CAnimation2DData(const CAnimation2DData& Anim);
	~CAnimation2DData();

public:
	inline void Update(float DeltaTime);

	void SetSequence(class CAnimationSequence2D* Seq, const std::string& Name,
		const std::string& SequenceName, float PlayTime, float PlayScale,
		EAnimLoopMethod LoopMethod, bool Reverse);

	inline void Save(FILE* File);
	inline void Load(FILE* File);
	CAnimation2DData* Clone();

private:
	class CAnimation2D* m_Owner;
	std::string	m_Name;
	std::string	m_SequenceName;
	CSharedPtr<CAnimationSequence2D>	m_Sequence;
	int		m_Frame;
	int		m_FrameCount;
	float	m_Time;
	float	m_FrameTime;
	float	m_PlayTime;
	float	m_PlayScale;
	EAnimLoopMethod	m_LoopMethod;
	bool	m_Reverse;
	bool	m_EndFunctionCalled;
	std::function<void()>	m_EndFunction;
	std::unordered_map<int, Animation2DNotify*>	m_mapNotify;

	//이 애니메이션의 재생이 끝나면 이어서 재생될 애니메이션의 이름(등록되어있어야 함)
	std::string m_SeriesAnimName;



public:
	inline const std::string& GetName()	const;
	inline int GetCurrentFrame()	const;
	inline float GetAnimationTime()	const;
	inline class CAnimationSequence2D* GetAnimationSequence()	const;
	inline const Animation2DFrameData* GetCurrentAnimationFrameData() const;
	inline void SetSeriesAnimation(const std::string& SeriesAnim);
	inline int GetFrameCount()	const;


	inline void SetLoopMethod(EAnimLoopMethod LoopMethod);
	inline void SetPlayTime(float PlayTime);
	inline void SetPlayScale(float PlayScale);
	inline void SetReverse(bool Reverse);

	inline const Animation2DFrameData* GetSCUnitFrameData(int Dir);
	
public:
	//기본값으로 되돌림
	void SetInitialValue();
	class CTexture* GetTexture() const;

public:
	template <typename T>
	void SetEndFunction(T* Obj, void(T::* Func)());

	template <typename T>
	void AddNotify(const std::string& Name, int Frame, T* Obj, void(T::* Func)());

	template <typename T>
	void AddNotify(const std::string& Name, float Time, T* Obj, void(T::* Func)());
};

inline const std::string& CAnimation2DData::GetName()	const
{
	return m_Name;
}

inline int CAnimation2DData::GetCurrentFrame()	const
{
	return m_Frame;
}

inline float CAnimation2DData::GetAnimationTime()	const
{
	return m_Time;
}

inline class CAnimationSequence2D* CAnimation2DData::GetAnimationSequence()	const
{
	return m_Sequence;
}

inline const Animation2DFrameData* CAnimation2DData::GetCurrentAnimationFrameData() const
{
	return m_Sequence->GetFrameData(m_Frame);
}

inline void CAnimation2DData::SetSeriesAnimation(const std::string& SeriesAnim)
{
	m_SeriesAnimName = SeriesAnim;
}

inline int CAnimation2DData::GetFrameCount() const
{
	return m_Sequence->GetFrameCount();
}

inline const Animation2DFrameData* CAnimation2DData::GetSCUnitFrameData(int Dir)
{
	return m_Sequence->GetFrameDataArrayIndexed(Dir, m_Frame);
}

inline void CAnimation2DData::SetLoopMethod(EAnimLoopMethod LoopMethod)
{
	m_LoopMethod = LoopMethod;
}

inline void CAnimation2DData::SetPlayTime(float PlayTime)
{
	m_PlayTime = PlayTime;
}

inline void CAnimation2DData::SetPlayScale(float PlayScale)
{
	m_PlayScale = PlayScale;
}

inline void CAnimation2DData::SetReverse(bool Reverse)
{
	m_Reverse = Reverse;
}




template <typename T>
void CAnimation2DData::SetEndFunction(T* Obj, void(T::* Func)())
{
	m_EndFunction = std::bind(Func, Obj);
}

template <typename T>
void CAnimation2DData::AddNotify(const std::string& Name, int Frame, T* Obj, void(T::* Func)())
{
	Animation2DNotify* Notify = new Animation2DNotify;

	Notify->Name = Name;
	Notify->Frame = Frame;
	Notify->Time = Frame * m_FrameTime;
	Notify->vecFunction.push_back(std::bind(Func, Obj));

	m_mapNotify.insert(Frame, Notify);
}

template <typename T>
void CAnimation2DData::AddNotify(const std::string& Name, float Time, T* Obj, void(T::* Func)())
{
	Animation2DNotify* Notify = new Animation2DNotify;

	Notify->Name = Name;
	Notify->Frame = Time / m_FrameTime;
	Notify->Time = Time;
	Notify->vecFunction.push_back(std::bind(Func, Obj));

	m_mapNotify.insert(Notify->Frame, Notify);
}