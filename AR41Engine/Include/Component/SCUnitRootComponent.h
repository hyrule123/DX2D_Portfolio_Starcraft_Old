#pragma once
#include "SceneComponent.h"

#include "../SCUnitInfo.h"

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


protected:
	//같은 유닛은 모두 동일한 유닛정보를 공유한다.(얕은 복사
	SCUnitInfo* m_SCUnitInfo;
	float m_DirAngleUnit;	//유닛 스프라이트의 방향 1단위의 각도
	INT8 m_Direction;

public:
	inline INT8 GetDirection() const;
	inline SCUnitInfo* GetSCUnitInfo() const;
};

inline INT8 CSCUnitRootComponent::GetDirection() const
{
	return m_Direction;
}

inline SCUnitInfo* CSCUnitRootComponent::GetSCUnitInfo() const
{
	return m_SCUnitInfo;
}
