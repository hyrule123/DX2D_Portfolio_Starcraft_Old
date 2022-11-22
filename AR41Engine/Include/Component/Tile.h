#pragma once

#include "../EngineInfo.h"

class CTile
{
	friend class CTileMapComponent;

private:
	CTile();
	CTile(const CTile& Tile);
	~CTile();

private:
	bool		m_Enable;
	class CTileMapComponent* m_Owner;
	ETileOption		m_TileOption;
	Vector3			m_Pos;
	Vector3			m_Size;
	Vector3			m_Center;
	Matrix			m_matWorld;
	float			m_Opacity;
	int				m_IndexX;
	int				m_IndexY;
	int				m_Index;
	Vector2			m_TileStart;
	Vector2			m_TileEnd;
	int				m_Frame;


public:
	inline void SetFrame(const int& Frame);
	inline int GetFrame() const;

	inline bool GetEnable()	const;

	inline ETileOption GetTileOption()	const;

	inline const Vector3& GetPos()	const;

	inline const Vector3& GetCenter()	const;

	inline const Matrix& GetWorldMatrix()	const;

	inline int GetIndexX()	const;

	inline int GetIndexY()	const;

	inline int GetIndex()	const;

	inline const Vector2& GetTileStart()	const;

	inline const Vector2& GetTileEnd()	const;

public:
	void Update(float DeltaTime);
	void UpdateTransform(float DeltaTime);
	CTile* Clone();
	void Save(FILE* File);
	void Load(FILE* File);
};

inline void CTile::SetFrame(const int& Frame)
{
	m_Frame = Frame;
}

inline int CTile::GetFrame() const
{
	return m_Frame;
}

inline bool CTile::GetEnable()	const
{
	return m_Enable;
}

inline ETileOption CTile::GetTileOption()	const
{
	return m_TileOption;
}

inline const Vector3& CTile::GetPos()	const
{
	return m_Pos;
}

inline const Vector3& CTile::GetCenter()	const
{
	return m_Center;
}

inline const Matrix& CTile::GetWorldMatrix()	const
{
	return m_matWorld;
}

inline int CTile::GetIndexX()	const
{
	return m_IndexX;
}

inline int CTile::GetIndexY()	const
{
	return m_IndexY;
}

inline int CTile::GetIndex()	const
{
	return m_Index;
}

inline const Vector2& CTile::GetTileStart()	const
{
	return m_TileStart;
}

inline const Vector2& CTile::GetTileEnd()	const
{
	return m_TileEnd;
}