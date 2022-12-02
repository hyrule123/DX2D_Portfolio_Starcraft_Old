#pragma once
#include "Component/SceneComponent.h"

enum class EUnitAction
{
	Idle,
	Move,
	Attack
};

class CSCUnitRootComponent :
    public CSceneComponent
{
	friend class CGameObject;
	friend class CScene;
	friend class CCDO;

protected:
	CSCUnitRootComponent();
	CSCUnitRootComponent(const CSCUnitRootComponent& component);
	virtual ~CSCUnitRootComponent();


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CSCUnitRootComponent* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

	inline INT8 GetDirection() const;

protected:
	float m_DirAngleUnit;	//유닛 스프라이트의 방향 1단위의 각도
	INT8 m_Direction;
};

inline INT8 CSCUnitRootComponent::GetDirection() const
{
	return m_Direction;
}


