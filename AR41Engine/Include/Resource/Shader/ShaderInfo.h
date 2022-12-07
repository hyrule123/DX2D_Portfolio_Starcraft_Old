#pragma once

#include "../../SCUnitInfo.h"



//============ ��Ÿũ����Ʈ ���� ���� ����ü �� ����ü ==============
//���������� �� ������ Ư�� ���̾��� �ؽ�ó ������ '����ϰ� �ִ���' ���θ� ����


struct SCUnit_CBufferStruct
{
	float Width;
	float Height;
	Vector2 Empty;
};

struct SCUnit_CBuffer
{
	//�� ������ '����������' � �ؽ�ó ��Ʈ�� ����ϰ� �ִ��� � ���� ������ ����
	//ESCUnit_TextureLayerFlags�� ���
	unsigned int RenderFlags;

	//���ֺ��� ����ϴ� �ؽ�ó�� ��ü ������� ������ۿ� ����
	//�ؽ�ó�� '��� �κ�'�� ����ȭ ���ۿ� ����
	SCUnit_CBufferStruct SCUnit_CBufferTexSizeInfo[(int)ESCUnit_TextureLayer::End];
};



enum ESCUnit_SBufferFlag : unsigned int
{
	//SCUnitShadowXFlip = 1 << 1,
	ESCUnitSelectedCircleRender = 1u << 0u,

	ESCUnitMainRender = 1u << 5u,
	ESCUnitMainXFlip = 1u << 6u,

	ESCUnitTopRender = 1u << 10u,
	ESCUnitTopXFlip = 1u << 11u,

	ESCUnitEffectRender = 1u << 15u,
	//SCUnitEffectXFlip = 1 << 16,

	ESCUnitBoosterRender = 1u << 20u,
	//SCUnitBoosterXFlip = 1 << 21,

	ESCUnitFlagAll = UINT_MAX
};
	
//�ؽ�ó�� '��� �κ�'�� �����Ѵ�.
struct SCUnit_SBufferStruct
{
	Vector2 Start;
	Vector2 End;
};

struct SCUnit_SBuffer
{
	Matrix  matWVP;

	Vector3 Pivot;
	float TransformEmpty1;

	Vector3  MeshSize;

	//ESCUnit_SBufferFlag �÷��� - �� ������ '����������' Ư�� �ؽ�ó�� ����ؾ� ���� ���θ� ����
	unsigned int SCUnit_SBufferFlag;

	//�ؽ�ó�� ��� �κ��� ����ȭ ���ۿ� ����
	Animation2DFrameData SCUnit_SBufferTexFrameInfo[(int)ESCUnit_TextureLayer::End];
};


// ��ġ, ���� ������ ������ �ִ� ����.
struct VertexColor
{
	Vector3	Pos;
	Vector4	Color;

	VertexColor()
	{
	}

	VertexColor(const Vector3& _Pos, const Vector4& _Color) :
		Pos(_Pos),
		Color(_Color)
	{
	}
};

struct VertexBuffer
{
	ID3D11Buffer* Buffer;
	int		Size;	// ���� 1���� ũ��
	int		Count;	// ���� ����
	void* Data;		// ���� ����

	VertexBuffer() :
		Buffer(nullptr),
		Size(0),
		Count(0),
		Data(nullptr)
	{
	}

	~VertexBuffer()
	{
		if (Data)
		{
			delete Data;
		}

		SAFE_RELEASE(Buffer);
	}
};

struct IndexBuffer
{
	ID3D11Buffer* Buffer;
	int		Size;		// �ε��� 1���� ũ��
	int		Count;		// �ε��� ����
	DXGI_FORMAT	Fmt;	// �ε��� ����
	void* Data;			// �ε��� ����

	IndexBuffer() :
		Buffer(nullptr),
		Size(0),
		Count(0),
		Fmt(DXGI_FORMAT_UNKNOWN),
		Data(nullptr)
	{
	}

	~IndexBuffer()
	{
		SAFE_DELETE_ARRAY(Data);
		SAFE_RELEASE(Buffer);
	}
};

struct TransformCBuffer
{
	Matrix  matWorld;
	Matrix  matView;
	Matrix  matProj;
	Matrix  matWVP;
	Vector3 Pivot;
	float	Empty1;
	Vector3 MeshSize;
	float	Empty2;
};

// ��ġ, ���� ������ ������ �ִ� ����.
struct VertexUV
{
	Vector3	Pos;
	Vector2	UV;

	VertexUV()
	{
	}

	VertexUV(const Vector3& _Pos, const Vector2& _UV) :
		Pos(_Pos),
		UV(_UV)
	{
	}
};

struct MaterialCBuffer
{
	Vector4 BaseColor;
	Vector4 AmbientColor;
	Vector4 SpecularColor;
	Vector4 EmissiveColor;
	float Opacity;
	int	TextureType;
	float TextureWidth;
	float TextureHeight;
	Vector3 ColorKey;
	float Empty;

	MaterialCBuffer() :
		ColorKey(-1.f, -1.f, -1.f),
		Opacity()
	{
	}
};


struct Animation2DCBuffer
{
	float Anim2DImageWidth;
	float Anim2DImageHeight;
	Vector2 Anim2DFrameStart;
	Vector2 Anim2DFrameEnd;
	int Anim2DType;
	int Anim2DEnable;
	int	Anim2DFrame;
	int Anim2DXFlip;
	Vector2 Anim2DEmpty;
};

struct ColliderCBuffer
{
	Vector4	Color;
	Matrix	matWVP;
};

struct UICBuffer
{
	Vector4 UITint;
	Matrix	UIWVP;
	Vector2 UIMeshSize;
	Vector2 UIPivot;
	int		UITextureEnable;
	float	UIOpacity;
	Vector2	UIEmpty;
};

struct UIProgressBarCBuffer
{
	int		BarDir;
	float	Percent;
	Vector2	Empty;
};


struct TileMapCBuffer
{
	Vector2	ImageSize;
	Vector2	Start;
	Vector2	End;
	Vector2	TileSize;
	Matrix	matWVP;
	int		Frame;
	Vector3	Empty;
};

struct TileInfo
{
	Matrix	matWVP;
	Vector2	Start;
	Vector2	End;
	Vector4	TypeColor;
	float	Opacity;
	int		AnimationType;
	int		Frame;
	float	Empty;

	TileInfo() :
		Opacity(1.f),
		TypeColor(1.f, 1.f, 1.f, 1.f),
		AnimationType(),
		Frame(0)
	{}
};
