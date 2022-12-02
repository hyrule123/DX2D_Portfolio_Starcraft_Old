#pragma once
#include "GameObject\GameObject.h"
class CSCUnitInstance :
    public CGameObject
{
	friend class CScene;
	friend class CCDO;

protected:
	CSCUnitInstance();
	CSCUnitInstance(const CSCUnitInstance& Obj);
	virtual ~CSCUnitInstance();

private:
	CSharedPtr<class CSCUnitRootComponent> m_UnitRoot;
	CSharedPtr<class CSCUltraliskComponent>	m_MainSprite;
	CSharedPtr<class CCameraComponent>	m_Camera;
	CSharedPtr<class CTargetArm>		m_Arm;
	CSharedPtr<class CColliderOBB2D>	m_Coll;
	CSharedPtr<class CAnimation2D> m_Anim;

public:
	virtual void Start();
	virtual bool Init();
	virtual bool CDOPreload();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CSCUnitInstance* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
public:
	void MoveUp();
	void MoveDown();
	void Rotation();
	void RotationInv();
	void Fire();
};

