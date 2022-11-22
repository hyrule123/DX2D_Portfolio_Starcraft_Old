#pragma once

#include "../Component/SceneComponent.h"
#include "../Component/ObjectComponent.h"

class CGameObject :
	public CCDO
{
	friend class CScene;
	friend class CSceneManager;
	friend class CCDO;

protected:
	CGameObject();
	CGameObject(const CGameObject& Obj);
	virtual ~CGameObject();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CGameObject* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

protected:
	class CScene* m_Scene;
	std::string		m_ObjectTypeName;
	int		m_ComponentSerialNumber;

public:
	inline class CScene* GetScene()    const;

	void SetScene(class CScene* Scene);

	inline const std::string& GetObjectTypeName()	const;

public:
	// 여기에 재정의하는 이유는 이 오브젝트가 제거될때 가지고 있는 모든 컴포넌트들도 제거를 해주기
	// 위해서이다.
	virtual void Destroy();

protected:
	CSharedPtr<CSceneComponent> m_RootComponent;
	std::list<CSceneComponent*> m_SceneComponentList;
	std::vector<CSharedPtr<CObjectComponent>>   m_vecObjectComponent;
	float       m_LifeTime;
	bool		m_Start;

public:
	inline void SetLifeTime(float LifeTime);

	inline void SetRootComponent(CSceneComponent* Component);

	inline void AddSceneComponent(CSceneComponent* Component);

	inline void DeleteSceneComponent(CSceneComponent* Component);

	void GetAllComponentHierarchyName(std::vector<HierarchyName>& vecName);

	inline CSceneComponent* GetRootComponent() const;

	inline const std::list<CSceneComponent*>& GetSceneComponents() const;

	CComponent* FindComponent(const std::string& Name);

	template <typename T>
	T* FindComponentFromType()  const;


public:
	template <typename T>
	T* CreateComponent(const std::string& Name);


public:
	void SetInheritScale(bool Inherit);
	void SetInheritRotX(bool Inherit);
	void SetInheritRotY(bool Inherit);
	void SetInheritRotZ(bool Inherit);
	void SetInheritParentRotationPosX(bool Inherit);
	void SetInheritParentRotationPosY(bool Inherit);
	void SetInheritParentRotationPosZ(bool Inherit);

	// 부모의 정보를 자식 Transform에 적용한다.
	void InheritRotation(bool Current);
	void InheritParentRotationPos();

	void InheritWorldRotation(bool Current);
	void InheritWorldParentRotationPos();

public:
	Vector3 GetRelativeScale()	const;
	Vector3 GetRelativeRot()	const;
	Vector3 GetRelativePos()	const;
	Vector3 GetRelativeAxis(AXIS Axis)	const;

public:
	void SetRelativeScale(const Vector3& Scale);
	void SetRelativeScale(const Vector2& Scale);
	void SetRelativeScale(float x, float y);
	void SetRelativeScale(float x, float y, float z);
	void SetRelativeScaleX(float x);
	void SetRelativeScaleY(float y);
	void SetRelativeScaleZ(float z);
	void SetRelativeRotation(const Vector3& Rot);
	void SetRelativeRotation(const Vector2& Rot);
	void SetRelativeRotation(float x, float y);
	void SetRelativeRotation(float x, float y, float z);
	void SetRelativeRotationX(float x);
	void SetRelativeRotationY(float y);
	void SetRelativeRotationZ(float z);
	void SetRelativePosition(const Vector3& Pos);
	void SetRelativePosition(const Vector2& Pos);
	void SetRelativePosition(float x, float y);
	void SetRelativePosition(float x, float y, float z);
	void SetRelativePositionX(float x);
	void SetRelativePositionY(float y);
	void SetRelativePositionZ(float z);

	void AddRelativeScale(const Vector3& Scale);
	void AddRelativeScale(const Vector2& Scale);
	void AddRelativeScale(float x, float y);
	void AddRelativeScale(float x, float y, float z);
	void AddRelativeScaleX(float x);
	void AddRelativeScaleY(float y);
	void AddRelativeScaleZ(float z);
	void AddRelativeRotation(const Vector3& Rot);
	void AddRelativeRotation(const Vector2& Rot);
	void AddRelativeRotation(float x, float y);
	void AddRelativeRotation(float x, float y, float z);
	void AddRelativeRotationX(float x);
	void AddRelativeRotationY(float y);
	void AddRelativeRotationZ(float z);
	void AddRelativePosition(const Vector3& Pos);
	void AddRelativePosition(const Vector2& Pos);
	void AddRelativePosition(float x, float y);
	void AddRelativePosition(float x, float y, float z);
	void AddRelativePositionX(float x);
	void AddRelativePositionY(float y);
	void AddRelativePositionZ(float z);

public:
	const Vector3& GetWorldScale()	const;
	const Vector3& GetWorldRot()	const;
	const Vector3& GetWorldPos()	const;
	const Vector3& GetWorldAxis(AXIS Axis)	const;
	const Vector3& GetPivot()	const;
	const Vector3& GetMeshSize()	const;
	const Matrix& GetWorldMatrix()	const;

public:
	void SetPivot(const Vector3& Pivot);
	void SetPivot(const Vector2& Pivot);
	void SetPivot(float x, float y, float z);
	void SetPivot(float x, float y);
	void SetMeshSize(const Vector3& MeshSize);
	void SetMeshSize(const Vector2& MeshSize);
	void SetMeshSize(float x, float y, float z);
	void SetMeshSize(float x, float y);

public:
	void SetWorldScale(const Vector3& Scale);
	void SetWorldScale(const Vector2& Scale);
	void SetWorldScale(float x, float y);
	void SetWorldScale(float x, float y, float z);
	void SetWorldScaleX(float x);
	void SetWorldScaleY(float y);
	void SetWorldScaleZ(float z);
	void SetWorldRotation(const Vector3& Rot);
	void SetWorldRotation(const Vector2& Rot);
	void SetWorldRotation(float x, float y);
	void SetWorldRotation(float x, float y, float z);
	void SetWorldRotationX(float x);
	void SetWorldRotationY(float y);
	void SetWorldRotationZ(float z);
	void SetWorldPosition(const Vector3& Pos);
	void SetWorldPosition(const Vector2& Pos);
	void SetWorldPosition(float x, float y);
	void SetWorldPosition(float x, float y, float z);
	void SetWorldPositionX(float x);
	void SetWorldPositionY(float y);
	void SetWorldPositionZ(float z);

	void AddWorldScale(const Vector3& Scale);
	void AddWorldScale(const Vector2& Scale);
	void AddWorldScale(float x, float y);
	void AddWorldScale(float x, float y, float z);
	void AddWorldScaleX(float x);
	void AddWorldScaleY(float y);
	void AddWorldScaleZ(float z);
	void AddWorldRotation(const Vector3& Rot);
	void AddWorldRotation(const Vector2& Rot);
	void AddWorldRotation(float x, float y);
	void AddWorldRotation(float x, float y, float z);
	void AddWorldRotationX(float x);
	void AddWorldRotationY(float y);
	void AddWorldRotationZ(float z);
	void AddWorldPosition(const Vector3& Pos);
	void AddWorldPosition(const Vector2& Pos);
	void AddWorldPosition(float x, float y);
	void AddWorldPosition(float x, float y, float z);
	void AddWorldPositionX(float x);
	void AddWorldPositionY(float y);
	void AddWorldPositionZ(float z);
};


inline class CScene* CGameObject::GetScene()    const
{
	return m_Scene;
}

inline const std::string& CGameObject::GetObjectTypeName()	const
{
	return m_ObjectTypeName;
}

inline void CGameObject::SetLifeTime(float LifeTime)
{
	m_LifeTime = LifeTime;
}

inline void CGameObject::SetRootComponent(CSceneComponent* Component)
{
	m_RootComponent = Component;
}

inline void CGameObject::AddSceneComponent(CSceneComponent* Component)
{
	Component->SetScene(m_Scene);
	Component->SetOwner(this);
	Component->Init();

	m_SceneComponentList.push_back(Component);
}

inline void CGameObject::DeleteSceneComponent(CSceneComponent* Component)
{
	auto	iter = m_SceneComponentList.begin();
	auto	iterEnd = m_SceneComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == Component)
		{
			m_SceneComponentList.erase(iter);
			return;
		}
	}
}

inline CSceneComponent* CGameObject::GetRootComponent() const
{
	return m_RootComponent;
}

inline const std::list<CSceneComponent*>& CGameObject::GetSceneComponents() const
{
	return m_SceneComponentList;
}

template <typename T>
T* CGameObject::FindComponentFromType()  const
{
	auto    iter = m_SceneComponentList.begin();
	auto    iterEnd = m_SceneComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->CheckTypeID<T>())
			return (T*)*iter;
	}

	auto    iter1 = m_vecObjectComponent.begin();
	auto    iter1End = m_vecObjectComponent.end();

	for (; iter1 != iter1End; ++iter1)
	{
		if ((*iter1)->CheckTypeID<T>())
			return (T*)(*iter1).Get();
	}

	return nullptr;
}

template <typename T>
T* CGameObject::CreateComponent(const std::string& Name)
{
	T* Component = ClonePLO<T>();

	Component->SetName(Name);
	Component->SetScene(m_Scene);
	Component->SetOwner(this);

	if (!Component->Init())
	{
		SAFE_RELEASE(Component);
		return nullptr;
	}

	if (Component->GetComponentType() == ComponentType::Object)
	{
		m_vecObjectComponent.push_back((CObjectComponent*)Component);
	}

	else
	{
		if (!m_RootComponent)
		{
			m_RootComponent = (CSceneComponent*)Component;
		}

		m_SceneComponentList.push_back((CSceneComponent*)Component);
	}

	Component->SetSerialNumber(m_ComponentSerialNumber);

	++m_ComponentSerialNumber;

	if (m_Start)
		Component->Start();

	return Component;
}