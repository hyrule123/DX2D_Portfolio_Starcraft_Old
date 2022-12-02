#pragma once

#include "../../EngineInfo.h"



//============ ��Ÿũ����Ʈ ���� ���� ����ü �� ����ü ==============
//���������� �� ������ Ư�� ���̾��� �ؽ�ó ������ '����ϰ� �ִ���' ���θ� ����
enum ESCUnit_CBufferRenderFlag : unsigned int
{
	//�׸��ڿ� ���� ���� ��ü�� ��Ȯ�� ������ ��������Ʈ�� ����ϹǷ� �״�� ���
	MainShadow = 1 << 0,
	UseShadowSprite = 1 << 1,

	//(���� ��)������ ��� �κ�
	Top = 1 << 2,

	//������ ����Ʈ �κ�
	Effect = 1 << 3,

	//������ �ν��� �κ�
	Attack = 1 << 4,
	Booster = 1 << 5
};

struct SCUnit_CBuffer
{
	//�� ������ '����������' � �ؽ�ó ��Ʈ�� ����ϰ� �ִ��� � ���� ������ ����
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

	//ESCUnit_SBufferFlag �÷��� - �� ������ '����������' Ư�� �ؽ�ó�� ����ؾ� ���� ���θ� ����
	unsigned int SCUnit_SBufferFlag;


	//�ִϸ��̼� ������ ����. �׸��ڰ� �ִ� ��������Ʈ�� ���� ���� ��ü�� ��Ȯ�� ��ġ��.
	//�׸��� ��������Ʈ�� ���� ���µ� �׸��ڸ� �������ϴ� ���� ��������Ʈ�� ���� ��ü ��������Ʈ�� �״�� �Ʒ��� �׷����� ������ ��������.
	float MainShadowWidth;
	float MainShadowHeight;
	Vector2 MainShadowEmpty;

	Vector2 MainShadowStart;
	Vector2 MainShadowEnd;


	//��� �κ�
	float TopWidth;
	float TopHeight;
	Vector2 TopEmpty;

	Vector2 TopStart;
	Vector2 TopEnd;


	//����Ʈ �κ�
	float EffectWidth;
	float EffectHeight;
	Vector2 EffectEmpty;

	Vector2 EffectStart;
	Vector2 EffectEnd;


	//�ν���/���� �κ�
	float AttackBoosterWidth;
	float AttackBoosterHeight;
	Vector2 AttackBoosterEmpty;

	Vector2 AttackBoosterStart;
	Vector2 AttackBoosterEnd;


	////���� �κ�
	//float AttackWidth;
	//float AttackHeight;
	//Vector2 AttackEmpty;

	//Vector2 AttackStart;
	//Vector2 AttackEnd;
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
