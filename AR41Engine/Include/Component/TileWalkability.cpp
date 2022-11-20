#include "TileWalkability.h"

#include "TileMapComponent.h"


CTileWalkability::CTileWalkability() :
	m_Owner(nullptr),
	m_Enable(true),
	m_TileOption(ETileOption::None),
	m_IndexX(0),
	m_IndexY(0),
	m_Index(0)
{
}

CTileWalkability::CTileWalkability(const CTileWalkability& Tile)
{
	*this = Tile;

	m_Owner = nullptr;
}

CTileWalkability::~CTileWalkability()
{
}

bool CTileWalkability::Init()
{
	return true;
}

void CTileWalkability::Update(float DeltaTime)
{
	// 애니메이션 연산
}

void CTileWalkability::UpdateTransform(float DeltaTime)
{

}

CTileWalkability* CTileWalkability::Clone()
{
	return new CTileWalkability(*this);
}

void CTileWalkability::Save(FILE* File)
{
	fwrite(&m_Enable, sizeof(bool), 1, File);

	fwrite(&m_TileOption, sizeof(ETileOption), 1, File);

	fwrite(&m_Pos, sizeof(Vector3), 1, File);
	fwrite(&m_Size, sizeof(Vector3), 1, File);
	fwrite(&m_Center, sizeof(Vector3), 1, File);

	fwrite(&m_IndexX, sizeof(int), 1, File);
	fwrite(&m_IndexY, sizeof(int), 1, File);
	fwrite(&m_Index, sizeof(int), 1, File);

}

void CTileWalkability::Load(FILE* File)
{
	fread(&m_Enable, sizeof(bool), 1, File);

	fread(&m_TileOption, sizeof(ETileOption), 1, File);

	fread(&m_Pos, sizeof(Vector3), 1, File);
	fread(&m_Size, sizeof(Vector3), 1, File);
	fread(&m_Center, sizeof(Vector3), 1, File);

	fread(&m_IndexX, sizeof(int), 1, File);
	fread(&m_IndexY, sizeof(int), 1, File);
	fread(&m_Index, sizeof(int), 1, File);
}
