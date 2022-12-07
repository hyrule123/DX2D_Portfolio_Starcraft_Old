#pragma once

#include "../../SCUnitInfo.h"



//============ 스타크래프트 유닛 관련 열거체 및 구조체 ==============
//공통적으로 한 유닛이 특정 레이어의 텍스처 정보를 '사용하고 있는지' 여부를 저장


struct SCUnit_CBufferStruct
{
	float Width;
	float Height;
	Vector2 Empty;
};

struct SCUnit_CBuffer
{
	//한 유닛이 '공통적으로' 어떤 텍스처 파트를 사용하고 있는지 등에 대한 정보를 저장
	//ESCUnit_TextureLayerFlags를 사용
	unsigned int RenderFlags;

	//유닛별로 사용하는 텍스처의 전체 사이즈는 상수버퍼에 저장
	//텍스처의 '출력 부분'은 구조화 버퍼에 저장
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
	
//텍스처의 '출력 부분'을 저장한다.
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

	//ESCUnit_SBufferFlag 플래그 - 한 유닛이 '개별적으로' 특정 텍스처를 출력해야 할지 여부를 저장
	unsigned int SCUnit_SBufferFlag;

	//텍스처의 출력 부분은 구조화 버퍼에 저장
	Animation2DFrameData SCUnit_SBufferTexFrameInfo[(int)ESCUnit_TextureLayer::End];
};


// 위치, 색상 정보를 가지고 있는 정점.
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
	int		Size;	// 정점 1개의 크기
	int		Count;	// 정점 개수
	void* Data;		// 정점 정보

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
	int		Size;		// 인덱스 1개의 크기
	int		Count;		// 인덱스 개수
	DXGI_FORMAT	Fmt;	// 인덱스 포멧
	void* Data;			// 인덱스 정보

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

// 위치, 색상 정보를 가지고 있는 정점.
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
