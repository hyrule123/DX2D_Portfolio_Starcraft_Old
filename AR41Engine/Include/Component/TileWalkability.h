#pragma once

#include "../EngineInfo.h"

class CTileWalkability
{
	friend class CTileMapComponent;

private:
	CTileWalkability();
	CTileWalkability(const CTileWalkability& Tile);
	~CTileWalkability();

public:
	bool Init();
	void Update(float DeltaTime);
	void UpdateTransform(float DeltaTime);
	CTileWalkability* Clone();
	void Save(FILE* File);
	void Load(FILE* File);

private:
	bool			m_Enable;
	class CTileMapComponent* m_Owner;
	ETileOption		m_TileOption;
	Vector3			m_Pos;
	Vector3			m_Size;
	Vector3			m_Center;
	int				m_IndexX;
	int				m_IndexY;
	int				m_Index;

public:
	ETileOption GetTileOption()	const
	{
		return m_TileOption;
	}

	const Vector3& GetPos()	const
	{
		return m_Pos;
	}

	const Vector3& GetCenter()	const
	{
		return m_Center;
	}

	int GetIndexX()	const
	{
		return m_IndexX;
	}

	int GetIndexY()	const
	{
		return m_IndexY;
	}

	int GetIndex()	const
	{
		return m_Index;
	}


};

