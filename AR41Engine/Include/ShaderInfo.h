#pragma once

#include "EngineInfo.h"



//============ ��Ÿũ����Ʈ ���� ���� ����ü �� ����ü ==============
//���������� �� ������ Ư�� ���̾��� �ؽ�ó ������ '����ϰ� �ִ���' ���θ� ����
enum class ERenderFlag : unsigned int
{
	//�׸��ڿ� ���� ���� ��ü�� ��Ȯ�� ������ ��������Ʈ�� ����ϹǷ� �״�� ���
	UnitMainShadow = 1 << 0,
	UseShadowSprite = 1 << 1,

	//(���� ��)������ ��� �κ�
	UnitTop = 1 << 2,

	//������ ����Ʈ �κ�
	UnitEffect = 1 << 3,

	//������ ����
	UnitAttackBooster = 1 << 4,

	//������ �ν��� �κ�
	UnitBooster = 1 << 5
};


struct SCUnitCBuffer
{
	//�� ������ '����������' � �ؽ�ó ��Ʈ�� ����ϰ� �ִ��� � ���� ������ ����
	unsigned int RenderFlags;
};

enum ESCUnitSBufferFlag : unsigned int
{
	SCUnitShadowRender = 1 << 0,
	SCUnitShadowXFlip = 1 << 1,


	SCUnitMainRender = 1 << 5,
	SCUnitMainXFlip = 1 << 6,	


	SCUnitTopRender = 1 << 10,
	SCUnitTopXFlip = 1 << 11,	


	SCUnitEffectRender = 1 << 15,
	SCUnitEffectXFlip = 1 << 16,


	SCUnitAttackRender = 1 << 20,
	SCUnitAttackXFlip = 1 << 21,


	SCUnitBoosterRender = 1 << 25,
	SCUnitBoosterXFlip = 1 << 26
};
	

struct SCUnitSBuffer
{
	Matrix  matWVP;

	Vector3 Pivot;
	float TransformEmpty1;

	Vector3  MeshSize;
	//�÷��� - �� ������ '����������' Ư�� �ؽ�ó�� ����ؾ� ���� ���θ� ����
	unsigned int SCUnitSBufferFlag;


	//�ִϸ��̼� ������ ����. �׸��ڿͰ� �ִ� ��������Ʈ�� ���� ���� ��ü�� ��Ȯ�� ��ġ��.
	//�׷��� ���� ��� ���̴��� ���ؼ� �ذ��� ���� �ҵ�
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



	//����/�ν��� �κ�
	float AttackWidth;
	float AttackHeight;
	Vector2 AttackEmpty;

	Vector2 AttackStart;
	Vector2 AttackEnd;


	//����/�ν��� �κ�
	float BoosterWidth;
	float BoosterHeight;
	Vector2 BoosterEmpty;

	Vector2 BoosterStart;
	Vector2 BoosterEnd;
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
