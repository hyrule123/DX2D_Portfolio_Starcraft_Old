#pragma once

#include "../../EngineInfo.h"



//============ 스타크래프트 유닛 관련 열거체 및 구조체 ==============
//공통적으로 한 유닛이 특정 레이어의 텍스처 정보를 '사용하고 있는지' 여부를 저장
enum ESCUnit_CBufferRenderFlag : unsigned int
{
	//그림자와 유닛 메인 몸체는 정확히 동일한 스프라이트를 사용하므로 그대로 출력
	MainShadow = 1 << 0,
	UseShadowSprite = 1 << 1,

	//(있을 시)유닛의 상단 부분
	Top = 1 << 2,

	//유닛의 이펙트 부분
	Effect = 1 << 3,

	//유닛의 부스터 부분
	Attack = 1 << 4,
	Booster = 1 << 5
};

struct SCUnit_CBuffer
{
	//한 유닛이 '공통적으로' 어떤 텍스처 파트를 사용하고 있는지 등에 대한 정보를 저장
	unsigned int RenderFlags;
};

enum ESCUnit_SBufferFlag : unsigned int
{
	//SCUnitShadowXFlip = 1 << 1,

	ESCUnitMainRender = 1u << 5u,
	ESCUnitMainXFlip = 1u << 6u,
	ESCUnitCloaked = 1u << 7u,

	ESCUnitTopRender = 1u << 10u,
	ESCUnitTopXFlip = 1u << 11u,

	ESCUnitEffectRender = 1u << 15u,
	//SCUnitEffectXFlip = 1 << 16,

	ESCUnitBoosterRender = 1u << 20u,
	//SCUnitBoosterXFlip = 1 << 21,

	ESCUnitAttackRender = 1u << 25u,
	//SCUnitAttackXFlip = 1 << 26

	ESCUnitFlagAll = UINT_MAX
};
	

struct SCUnit_SBuffer
{
	Matrix  matWVP;

	Vector3 Pivot;
	float TransformEmpty1;

	Vector3  MeshSize;

	//ESCUnit_SBufferFlag 플래그 - 한 유닛이 '개별적으로' 특정 텍스처를 출력해야 할지 여부를 저장
	unsigned int SCUnit_SBufferFlag;


	//애니메이션 정보를 저장. 그림자가 있는 스프라이트는 메인 유닛 몸체와 정확히 일치함.
	//그림자 스프라이트가 따로 없는데 그림자를 렌더링하는 유닛 스프라이트는 유닛 본체 스프라이트를 그대로 아래에 그려내는 식으로 렌더링함.
	float MainShadowWidth;
	float MainShadowHeight;
	Vector2 MainShadowEmpty;

	Vector2 MainShadowStart;
	Vector2 MainShadowEnd;


	//상단 부분
	float TopWidth;
	float TopHeight;
	Vector2 TopEmpty;

	Vector2 TopStart;
	Vector2 TopEnd;


	//이펙트 부분
	float EffectWidth;
	float EffectHeight;
	Vector2 EffectEmpty;

	Vector2 EffectStart;
	Vector2 EffectEnd;


	//부스터/공격 부분
	float AttackBoosterWidth;
	float AttackBoosterHeight;
	Vector2 AttackBoosterEmpty;

	Vector2 AttackBoosterStart;
	Vector2 AttackBoosterEnd;


	////공격 부분
	//float AttackWidth;
	//float AttackHeight;
	//Vector2 AttackEmpty;

	//Vector2 AttackStart;
	//Vector2 AttackEnd;
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
