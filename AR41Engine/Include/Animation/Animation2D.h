#pragma once

#include "../Ref.h"
#include "Animation2DData.h"

class CAnimation2D :
    public CRef
{
	friend class CUnitSpriteComponent;
	friend class CSCUnitSpriteComponent;
    friend class CSpriteComponent;
	friend class CSceneManager;
	friend class CScene;

protected:
    CAnimation2D();
    CAnimation2D(const CAnimation2D& Anim);
    virtual ~CAnimation2D();

public://Default
	void Start();
	virtual bool Init();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
	virtual CAnimation2D* Clone();

	//애니메이션 변경 메소드
	void SetCurrentAnimation(const std::string& Name);
	void ChangeAnimation(const std::string& Name);

protected:
	std::string	m_ClassName;
	class CSpriteComponent* m_Owner;
    std::unordered_map<std::string, class CAnimation2DData*>  m_mapAnimation;
    class CAnimation2DData* m_CurAnimation;
	
	bool	m_Play;
	bool	m_UseInstancing;


public:
	
	inline class CScene* GetScene()	const;
	inline const std::string& GetAnimationClassName()	const;
	inline void GetAnimationNames(std::vector<std::string>& vecNames);
	class CAnimation2DData* GetCurrentAnimation() const;
	int GetCurAnimMaterialTextureInfoPreset() const;
	inline bool IsPlaying() const;
	void SetInstancing(bool b) { m_UseInstancing = b; }

	void SetOwner(class CSpriteComponent* Owner);

public:
	virtual void Update(float DeltaTime);

	bool AddAnimation(const std::string& Name, const std::string& SequenceName,
		float PlayTime = 1.f, float PlayScale = 1.f,
		EAnimLoopMethod LoopMethod = EAnimLoopMethod::NoLoop, bool Reverse = false);
	bool AddAnimation(const std::string& Name,  class CAnimationSequence2D* Sequence, float PlayTime = 1.f, float PlayScale = 1.f,
		EAnimLoopMethod LoopMethod = EAnimLoopMethod::NoLoop, bool Reverse = false);


	void SetPlayTime(const std::string& Name, float PlayTime);
	void SetPlayScale(const std::string& Name, float PlayScale);
	void SetLoop(const std::string& Name, EAnimLoopMethod LoopMethod);
	void SetReverse(const std::string& Name, bool Reverse);


	//일반 상수버퍼 사용하여 렌더링
	void SetShader();

public:
	CAnimation2DData* FindAnimation(const std::string& Name);

public:
	std::string GetCurrentAnimationName()	const;
	int GetCurrentFrame()	const;
	float GetCurrentAnimationTime()	const;
	class CAnimationSequence2D* GetCurrentAnimationSequence()	const;
	const Animation2DFrameData* GetCurrentAnimationFrameData()	const;
	const Animation2DFrameData* GetCurrentAnimationFrameDataSCUnit(int Dir) const;


	void ChangeTexture(class CTexture* Tex);


public:
	template <typename T>
	void SetCurrentEndFunction(const std::string& AnimationName, T* Obj, void(T::* Func)());

	template <typename T>
	void AddCurrentNotify(const std::string& AnimationName,
		const std::string& Name, int Frame, T* Obj, void(T::* Func)());

	template <typename T>
	void AddCurrentNotify(const std::string& AnimationName,
		const std::string& Name, float Time, T* Obj, void(T::* Func)());
};

inline const std::string& CAnimation2D::GetAnimationClassName()	const
{
	return m_ClassName;
}

inline void CAnimation2D::GetAnimationNames(std::vector<std::string>& vecNames)
{
	auto	iter = m_mapAnimation.begin();
	auto	iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		vecNames.push_back(iter->first);
	}
}






template <typename T>
void CAnimation2D::SetCurrentEndFunction(const std::string& AnimationName, T* Obj, void(T::* Func)())
{
	CAnimation2DData* Animation = FindAnimation(AnimationName);

	if (!Animation)
		return;

	Animation->SetEndFunction<T>(Obj, Func);
}

template <typename T>
void CAnimation2D::AddCurrentNotify(const std::string& AnimationName,
	const std::string& Name, int Frame, T* Obj, void(T::* Func)())
{
	CAnimation2DData* Animation = FindAnimation(AnimationName);

	if (!Animation)
		return;

	Animation->AddNotify<T>(Name, Frame, Obj, Func);
}

template <typename T>
void CAnimation2D::AddCurrentNotify(const std::string& AnimationName,
	const std::string& Name, float Time, T* Obj, void(T::* Func)())
{
	CAnimation2DData* Animation = FindAnimation(AnimationName);

	if (!Animation)
		return;

	Animation->AddNotify<T>(Name, Time, Obj, Func);
}

inline bool CAnimation2D::IsPlaying() const
{
	return m_Play;
}
