#pragma once
#include "Component/SceneComponent.h"

enum class EUnitAction
{
	Idle,
	Move,
	Attack
};

class CUnitRootComponent :
    public CSceneComponent
{
	friend class CGameObject;
	friend class CScene;
	friend class CCDO;

protected:
	CUnitRootComponent();
	CUnitRootComponent(const CUnitRootComponent& component);
	virtual ~CUnitRootComponent();


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CUnitRootComponent* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

	inline INT8 GetDirection() const;;
	inline bool GetSpriteFlip() const;

protected:
	float m_DirAngleUnit;	//유닛 스프라이트의 방향 1단위의 각도
	INT8 m_Direction;
};

inline INT8 CUnitRootComponent::GetDirection() const
{
	return m_Direction;
}


