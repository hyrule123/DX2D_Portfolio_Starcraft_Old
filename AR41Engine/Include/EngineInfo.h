
#pragma once

#define	DIRECTINPUT_VERSION	0x0800

#include <Windows.h>
#include <vector>
#include <list>
#include <unordered_map>
#include <crtdbg.h>
#include <typeinfo>
#include <string>
#include <functional>
#include <algorithm>
#include <stack>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <filesystem>
#include <dwrite_3.h>
#include <d2d1.h>
#include <process.h>



#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "SharedPtr.h"
#include "Resource/Texture/DirectXTex.h"
#include "fmod.hpp"


#ifdef _DEBUG
#include "lodepng/lodepng.h"

#else


#endif


#ifndef STORMLIB_NO_AUTO_LINK
#define STORMLIB_NO_AUTO_LINK
#endif

extern float g_DeltaTime;

#ifdef _UNICODE
#define lfopen_s _wfopen_s
typedef std::wstring tstring;
#else
#define lfopen_s fopen_s
typedef std::string tstring;
#endif

//경로
#define	ROOT_PATH		"RootPath"
#define	SHADER_PATH		"ShaderPath"
#define	TEXTURE_PATH	"TexturePath"
#define	SOUND_PATH		"SoundPath"
#define	FONT_PATH		"FontPath"
#define	ANIMATION2D_PATH "Animation2DPath"
#define	SCENE_PATH		"ScenePath"
#define MAP_PATH		"MapPath"
#define CDO_PATH		"CDOPath"


//리소스 종류
#define MESH "MESH"
#define SHADER "SHADER"
#define CBUFFER "CBUFFER"
#define TEXTURE "TEXTURE"
#define TEXTUREARRAY "TEXTUREARRAY"
#define MATERIAL "MATERIAL"
#define ANIMATION "ANIMATION"
#define SOUND "SOUND"
#define FONT "FONT"
#define FONTCOLLECTION "FONTCOLLECTON"
#define MAP "MAP"
#define UNITINFO "UNITINFO"
#define RESOURCEEND "RESEND"



#define	DECLARE_SINGLE(Type)	\
private:\
	static Type*	m_Inst;\
public:\
	static Type* GetInst()\
	{\
		if (!m_Inst)\
			m_Inst = new Type;\
		return m_Inst;\
	}\
	static void DestroyInst()\
	{\
		if(m_Inst)\
		{\
			delete m_Inst;\
			m_Inst = nullptr;\
		}\
	}\
private:\
	Type();\
	~Type();

#define	DEFINITION_SINGLE(Type)	Type* Type::m_Inst = nullptr;

#define	SAFE_DELETE(p)	if(p)	{ delete p; p = nullptr; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = nullptr; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = nullptr;}

struct Resolution
{
	unsigned int	Width;
	unsigned int	Height;
};


//동적 2d array를 쉽게 다루기 위한 클래스.
template <typename T>
class Array2D {
public:

	Array2D(T* ptr, int pitch) :
		_ptr(ptr),
		_pitch(pitch)
	{}

	T* operator[](int idx)
	{
		return _ptr + (_pitch * idx);
	}

private:
	T* const _ptr;
	const int _pitch;
};


struct HierarchyName
{
	std::string	Name;
	std::string	ParentName;
	std::string	ClassName;
	std::string	ParentClassName;
	class CComponent* Component;
	class CComponent* Parent;
};

struct HierarchyObjectName
{
	std::string	Name;
	std::string	ParentName;
	std::string	ClassName;
	std::string	ParentClassName;
	class CGameObject* Obj;
	class CGameObject* Parent;
};

struct Animation2DFrameData
{
	Vector2	Start;
	Vector2	End;
};



struct CollisionChannel
{
	std::string			Name;
	ECollision_Channel	Channel;
	ECollision_Interaction	Interaction;
};

struct CollisionProfile
{
	std::string							Name;
	CollisionChannel*					Channel;
	bool								Enable;
	std::vector<ECollision_Interaction>	vecCollisionInteraction;

	CollisionProfile() :
		Enable(true)
	{
	}
};

struct CollisionResult
{
	class CCollider* Src;
	class CCollider* Dest;
	Vector3			HitPoint;

	CollisionResult() :
		Src(nullptr),
		Dest(nullptr)
	{
	}
};



struct Box2DInfo
{
	float	Left;
	float	Bottom;
	float	Right;
	float	Top;
};

struct Sphere2DInfo
{
	Vector2	Center;
	float	Radius;
};

struct OBB2DInfo
{
	Vector2	Center;
	Vector2	Axis[AXIS2D_MAX];
	float	Length[AXIS2D_MAX];
};

struct PixelInfo
{
	Box2DInfo	Box2D;
	unsigned char* Pixel;
	unsigned int	Width;
	unsigned int	Height;
	EPixelCollision_Type	PixelColorCollisionType;
	EPixelCollision_Type	PixelAlphaCollisionType;
	unsigned char	TypeColor[4];
	ID3D11ShaderResourceView* SRV;
	std::string	Name;
	TCHAR		FileName[MAX_PATH];
	char		PathName[MAX_PATH];
	int			RefCount;

	PixelInfo()	:
		FileName{},
		PathName{},
		RefCount(0),
		Pixel(nullptr),
		Box2D{},
		Width(0),
		Height(0),
		PixelColorCollisionType(EPixelCollision_Type::Alpha_Confirm),
		PixelAlphaCollisionType(EPixelCollision_Type::None),
		TypeColor{}
	{
	}
};



struct ThreadSyncData
{
	int	Header;
	int	Size;
	unsigned char Data[1024];
};


//PreLoad 시 사용할 정보
struct RequiredResource
{
	EResourceType ResType;
	std::string Name;
	tstring FileName;
};
enum class ERequiredResource
{
	ResType,
	Name,
	FileName
};


enum class EAnimLoopMethod
{
	NoLoop,
	NormalLoop,
	LoopZigZag
};