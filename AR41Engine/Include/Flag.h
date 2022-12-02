#pragma once

enum AXIS2D
{
	AXIS2D_X,
	AXIS2D_Y,
	AXIS2D_MAX
};

enum AXIS
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_MAX
};

enum class MeshType : unsigned char
{
	Sprite,
	UI,
	Static,
	Animation
};

enum class BufferType : unsigned char
{
	Vertex,
	Index
};

enum class ShaderType : unsigned char
{
	Graphic,
	Compute
};

enum class ComponentType : unsigned char
{
	Scene,
	Object
};

enum class SceneComponentType : unsigned char
{
	Sprite
};

enum class EShaderBufferType : unsigned char
{
	Vertex = 0x1,
	Pixel = 0x2,
	Domain = 0x4,
	Hull = 0x8,
	Geometry = 0x10,
	Compute = 0x20,
	Graphic = Vertex | Pixel | Domain | Hull | Geometry,
	All = Vertex | Pixel | Domain | Hull | Geometry | Compute
};

enum class ECameraType : unsigned char
{
	Camera2D,
	Camera3D,
	CameraUI
};

enum class EImageType : unsigned char
{
	Atlas,
	Frame,
	Array,
	//동일한 사이즈의 텍스처를 동시에 들고 있음(동시 출력)
	Simultaneous
};

enum class ESamplerType : unsigned char
{
	Point,
	Linear,
	Anisotropic
};

enum class ERenderStateType : unsigned char
{
	Blend,
	Rasterizer,
	DepthStencil,
	End
};

enum class EAnimation2DType : int
{
	None = -1,
	Atlas,
	Frame,
	Array,
	AtlasIndexed
};

enum class ECollision_Channel
{
	Default,
	Mouse,
	Custom0,
	Custom1,
	Custom2,
	Custom3,
	Custom4,
	Custom5,
	Custom6,
	Custom7,
	Custom8,
	Custom9,
	Custom10,
	Custom11,
	Custom12,
	Custom13,
	Custom14,
	Custom15,
	Custom16,
	Max
};

enum class ECollision_Interaction
{
	Ignore,
	Collision
};

enum class ECollider_Type
{
	Collider2D,
	Collider3D
};

enum class ECollider2D_Type
{
	Box2D,	// 회전없는 상자
	OBB2D,	// 회전되는 상자
	Sphere2D,	// 원
	Pixel	// 픽셀충돌용
};

enum class ECollider3D_Type
{
	Box,	// 회전없는 상자
	OBB,	// 회전되는 상자
	Sphere	// 원
};

enum class ECollision_Result
{
	Collision,
	Release,
	Max
};

enum class EPixelCollision_Type
{
	None,
	Color_Ignore,
	Color_Confirm,
	Alpha_Ignore,
	Alpha_Confirm
};

enum class EProgressBarTextureType
{
	Back,
	Bar,
	Max
};

enum class EProgressBarDir
{
	RightToLeft,
	LeftToRight,
	TopToBottom,
	BottomToTop
};


enum class ESceneComponentType
{
	Scene,
	Primitive,
	Sprite,
	Camera,
	TargetArm,
	Collider,
	Collider2D,
	Box2D,
	OBB2D,
	Sphere2D,
	Pixel,
	Collider3D,
	TileMap,
	Max
};

enum class ETileShape
{
	Rect,
	Isometric
};

enum class ETileOption
{
	None,
	Wall,
	End
};

enum class EResourceType
{
	None = -1,
	Mesh,
	Shader,
	CBuffer,
	Texture,
	Material,
	Animation,
	Sound,
	Font,
	FontCollection,
	Map,
	SCUnitInfo,
	End
};


//맵데이터로부터 로드해야하는 데이터들의 플래그
enum EMapDataType
{
	Terrain,
	MapSize,
	TileMap,
	EMapDataTypeEnd
};

enum ESCUnitLayers
{
	SCUnitShadow,
	SCUnitBackLayer,
	SCUnitFrontLayer,
	SCUnitBoostEffectLayer,
	SCUnitAttackEffectLayer
};