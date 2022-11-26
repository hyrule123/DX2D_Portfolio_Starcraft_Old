#pragma once

#include "../EngineInfo.h"
#include "../Resource/Animation/AnimationSequence2D.h"

struct Animation2DNotify
{
	std::string	Name;
	int		Frame;
	float	Time;
	bool	Call;
	std::function<void()>	Function;

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

private:
	class CAnimation2D* m_Owner;
	std::string	m_Name;

	//프레임이 동일한 같은 애니메이션을 여러개 재생할수 있도록 vector 형태로 전환
	//재생은 일괄적으로 함. 따로 떼서 재생하고 그런거 X
	//모두 같은 Row 수를 가지고 있다고 가정하고 사용함
	std::vector<CSharedPtr<CAnimationSequence2D>>	m_vecSequence;
	int		m_Frame;
	float	m_Time;
	float	m_FrameTime;
	float	m_PlayTime;
	float	m_PlayScale;
	bool	m_Loop;
	bool	m_Reverse;
	std::function<void()>	m_EndFunction;
	std::vector<Animation2DNotify*>	m_vecNotify;


public:
	void Update(float DeltaTime);
	void SetSequence(CAnimationSequence2D* Sequence, int Index = 0);
	void AddSequence(CAnimationSequence2D* Sequence);
	void Save(FILE* File);
	void Load(FILE* File);
	CAnimation2DData* Clone();

public://Inline
	inline const std::string& GetName()	const;
	inline int GetCurrentFrame()	const;
	inline float GetAnimationTime()	const;
	inline class CAnimationSequence2D* GetAnimationSequence(int Index = 0)	const;
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

inline CAnimationSequence2D* CAnimation2DData::GetAnimationSequence(int Index)	const
{
	if (Index >= (int)m_vecSequence.size())
		return nullptr;


	return m_vecSequence[Index];
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
	Notify->Function = std::bind(Func, Obj);

	m_vecNotify.push_back(Notify);
}

template <typename T>
void CAnimation2DData::AddNotify(const std::string& Name, float Time, T* Obj, void(T::* Func)())
{
	Animation2DNotify* Notify = new Animation2DNotify;

	Notify->Name = Name;
	Notify->Frame = Time / m_FrameTime;
	Notify->Time = Time;
	Notify->Function = std::bind(Func, Obj);

	m_vecNotify.push_back(Notify);
}

