#include "Tile.h"
#include "TileMapComponent.h"

CTile::CTile()	:
	m_Owner(nullptr),
	m_Enable(true),
	m_TileOption(ETileOption::None),
	m_IndexX(0),
	m_IndexY(0),
	m_Index(0),
	m_Frame(),
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

void CTile::Save(FILE* File)
{
	fwrite(&m_Enable, sizeof(bool), 1, File);

	fwrite(&m_TileOption, sizeof(ETileOption), 1, File);

	fwrite(&m_Pos, sizeof(Vector3), 1, File);
	fwrite(&m_Size, sizeof(Vector3), 1, File);
	fwrite(&m_Center, sizeof(Vector3), 1, File);

	fwrite(&m_IndexX, sizeof(int), 1, File);
	fwrite(&m_IndexY, sizeof(int), 1, File);
	fwrite(&m_Index, sizeof(int), 1, File);

	fwrite(&m_TileStart, sizeof(Vector2), 1, File);
	fwrite(&m_TileEnd, sizeof(Vector2), 1, File);

}

void CTile::Load(FILE* File)
{
	fread(&m_Enable, sizeof(bool), 1, File);

	fread(&m_TileOption, sizeof(ETileOption), 1, File);

	fread(&m_Pos, sizeof(Vector3), 1, File);
	fread(&m_Size, sizeof(Vector3), 1, File);
	fread(&m_Center, sizeof(Vector3), 1, File);

	fread(&m_IndexX, sizeof(int), 1, File);
	fread(&m_IndexY, sizeof(int), 1, File);
	fread(&m_Index, sizeof(int), 1, File);

	fread(&m_TileStart, sizeof(Vector2), 1, File);
	fread(&m_TileEnd, sizeof(Vector2), 1, File);

}
