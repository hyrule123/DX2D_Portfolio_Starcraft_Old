#pragma once

#include "PrimitiveComponent.h"
#include "Tile.h"


/*
//
//Ÿ�ϸ� ������Ʈ ��� ����
//

1. ��ġ�� ����Ǹ� ��� Ÿ�ϸ��� ��ġ�� �ٽð���ؾ� �ϹǷ� �ݵ�� ��ġ ���� ����ٰ�
2. Ÿ�ϸ� ����
3. �̵����� Ÿ�ϸ� ����


*/

typedef struct TileWalkability
{
    ETileOption TileOption;
    Vector2 Pos;
    Vector2 Center;
    int IndexX;
    int IndexY;
    int Index;

} TileWalkability;

class CTileMapComponent :
    public CPrimitiveComponent
{
    friend class CCDO;
    friend class CGameObject;
    friend class CScene;

protected:
    CTileMapComponent();
    CTileMapComponent(const CTileMapComponent& component);

public:
    virtual ~CTileMapComponent();

public:
    virtual bool CDOPreload();
    virtual bool Init();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CTileMapComponent* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

protected:
    //Ÿ�ϸ� ���� ����
    CSharedPtr<class CTexture>  m_TileBackTexture;

    //Ÿ�� �⺻ ����
    std::string m_SceneName;
    ETileShape      m_Shape;

    //��¿� Ÿ�� ����
    class CTileMapConstantBuffer* m_TileMapCBuffer;
    class CStructuredBuffer* m_TileInfoBuffer;
    std::vector<CTile*> m_vecTile;
    std::vector<TileInfo>   m_vecTileInfo;
    CSharedPtr<class CMesh> m_TileMesh;
    CSharedPtr<class CMaterial>  m_TileMaterial;
    int     m_CountX;
    int     m_CountY;
    int     m_Count;
    int     m_TileInfoCount;
    int     m_RenderCount;
    Vector2 m_TileSize;
    Vector4 m_TileTypeColor[(int)ETileOption::End];
    Vector2 m_TileStartFrame;
    Vector2 m_TileEndFrame;
    class CTile* m_EditorMouseOnTile;

    //Array �Ǵ� Atlas �ؽ�ó�� ��� �ؽ�ó�� ��� �κ��� ����ؾ����� 
    std::vector<Animation2DFrameData>   m_vecTileFrame;

    //��ã��� Ÿ�� ����(Walkability Map)
    std::vector<TileWalkability> m_vecTileWalkability;
    int m_WalkabilityCountX;
    int m_WalkabilityCountY;
    int m_WalkabilityCount;
    Vector2 m_TileWalkabilitySize;

public:
    void CreateTileWalkibility(int WalkabilityCountX, int WalkabilityCountY,
        Vector2 TileWalkabilitySize = Vector2(8.f, 8.f));
    inline void SetTileWalkability(int IndexX, int IndexY, ETileOption TileOption);

protected:
    //����׿� �� �޽�
    CSharedPtr<class CMesh> m_TileDebugLineMesh;



public:
    inline const std::string& GetSceneName()   const;
    inline int GetCountX() const;
    inline int GetCountY() const;
    inline int GetCount() const;
    inline ETileShape GetShape()   const;
    inline const Vector2& GetTileSize()    const;
    inline class CMaterial* GetTileMaterial()  const;
    

public:
    void SetEditorMouseOnTile(int Index);
    void SetTileMaterial(const std::string& Name);
    void SetTileMaterial(class CMaterial* Material);
    void SetTileTexture(class CTexture* Texture);
    void SetTileTexture(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    void SetTileTextureFullPath(const std::string& Name, const TCHAR* FullPath);
    void SetTileTextureArray(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    void SetTileTextureArrayFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);


    void SetTileBackTexture(class CTexture* Texture);
    void SetTileBackTexture(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    void SetTileBackTextureFullPath(const std::string& Name, const TCHAR* FullPath);
    void SetTileBackTextureArray(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    void SetTileBackTextureArrayFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);

    void SetTileBaseColor(const Vector4& Color);
    void CreateTile(ETileShape Shape, int CountX, int CountY,
        const Vector2& TileSize);


public:
    int GetTileIndexX(const Vector2& Pos);
    int GetTileIndexX(const Vector3& Pos);
    int GetTileIndexY(const Vector2& Pos);
    int GetTileIndexY(const Vector3& Pos);
    int GetTileIndex(const Vector2& Pos);
    int GetTileIndex(const Vector3& Pos);

    CTile* GetTile(const Vector2& Pos);
    CTile* GetTile(const Vector3& Pos);
    CTile* GetTile(int X, int Y);
    CTile* GetTile(int Index);
    int GetTileTextureFrame(int X, int Y);
    int GetTileTextureFrame(int TileIndex);

    void GetWalkabilityRegion(int StartX, int StartY, int SizeX, int SizeY, __out std::vector<ETileOption>&);

private:
    int GetTileRenderIndexX(const Vector3& Pos);
    int GetTileRenderIndexY(const Vector3& Pos);


public:
    void ChangeTileFrame(const Vector2& Pos, int Frame);
    void ChangeTileOption(const Vector2& Pos, ETileOption Option);

};

inline const std::string& CTileMapComponent::GetSceneName()   const
{
    return m_SceneName;
}

inline int CTileMapComponent::GetCountX() const
{
    return m_CountX;
}

inline int CTileMapComponent::GetCountY() const
{
    return m_CountY;
}

inline int CTileMapComponent::GetCount() const
{
    return m_Count;
}

inline ETileShape CTileMapComponent::GetShape()   const
{
    return m_Shape;
}

inline const Vector2& CTileMapComponent::GetTileSize()    const
{
    return m_TileSize;
}

inline class CMaterial* CTileMapComponent::GetTileMaterial()  const
{
    return m_TileMaterial;
}



inline void CTileMapComponent::SetTileWalkability(int IndexX, int IndexY, ETileOption TileOption)
{
    if (IndexX < 0 || IndexX > m_WalkabilityCountX || IndexY < 0 || IndexY > m_WalkabilityCountY)
        return;

    m_vecTileWalkability[IndexY * m_WalkabilityCountX + IndexX].TileOption = TileOption;
}
