#include "Tile.h"
#include "TileMapComponent.h"

CTile::CTile()	:
	m_Owner(nullptr),
	m_Enable(true),
	m_TileOption(ETileOption::None),
	m_IndexX(0),
	m_IndexY(0),
	m_Index(0),
	m_Anim2DType(EAnimation2DType::None),
	m_Frame(0),
	m_Time(0.f),
	m_FrameTime(0.f),
	m_PlayTime(1.f),
	m_PlayScale(1.f),
	m_Loop(true),
	m_Reverse(false),
	m_Opacity(1.f)
{
}

CTile::CTile(const CTile& Tile)
{
	*this = Tile;

	m_Owner = nullptr;
}

CTile::~CTile()
{
}

void CTile::Update(float DeltaTime)
{
	// 애니메이션 연산
}

void CTile::UpdateTransform(float DeltaTime)
{
	// 트랜스폼 연산
	Vector3	OwnerPos = m_Owner->GetWorldPos();

	Vector3	Pos = OwnerPos + m_Pos;

	Matrix	matScale, matTranslate;

	matScale.Scaling(m_Size.x, m_Size.y, 1.f);
	matTranslate.Translation(Pos);

	m_matWorld = matScale * matTranslate;
}

CTile* CTile::Clone()
{
	return new CTile(*this);
}
