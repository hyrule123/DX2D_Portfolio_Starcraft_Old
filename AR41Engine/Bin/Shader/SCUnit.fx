
#include "Share.fx"




struct VS_INPUT_UV
{
	float3 Pos : POSITION;
	float2 UV : TEXCOORD;
	uint InstanceID : SV_InstanceID;
};

struct VS_OUTPUT_UV
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
	float4 Color : COLOR;
};




//레이어 번호(배열 인덱스 용)
#define	ESCUnit_TextureLayer_Selected 0u
#define	ESCUnit_TextureLayer_MainShadow 1u
#define	ESCUnit_TextureLayer_Top 2u
#define	ESCUnit_TextureLayer_Effect 3u
#define	ESCUnit_TextureLayer_Booster 4u
#define	ESCUnit_TextureLayer_End 5u

//텍스처 레이어 플래그 값
//플래그 값
#define	ESCUnit_TextureLayerFlag_Selected  1 << 0
#define	ESCUnit_TextureLayerFlag_Main 1 << 1
#define	ESCUnit_TextureLayerFlag_Top  1 << 2
#define	ESCUnit_TextureLayerFlag_Effect  1 << 3
#define	ESCUnit_TextureLayerFlag_Booster  1 << 4
#define	ESCUnit_TextureLayerFlag_UseShadowSprite  1 << 5
#define	ESCUnit_TextureLayerFlag_FlagCount  6
#define	ESCUnit_TextureLayerFlag_All  0xff

struct SCUnit_CBufferStruct
{
    float Width;
    float Height;
    float2 Empty;
};

cbuffer SCUnitCBuffer : register(b12)
{
    //한 유닛이 '공통적으로' 어떤 텍스처 파트를 사용하고 있는지 등에 대한 정보를 저장
	//ESCUnit_TextureLayerFlags를 사용
    unsigned int RenderFlags;
    float3 Empty;

	//유닛별로 사용하는 텍스처의 전체 사이즈는 상수버퍼에 저장
	//텍스처의 '출력 부분'은 구조화 버퍼에 저장
    SCUnit_CBufferStruct SCUnit_CBufferTexSizeInfo[(int)ESCUnit_TextureLayer_End];
};

struct Animation2DFrameData
{
    float2 Start;
    float2 End;
};

struct SCUnit_SBuffer
{
    Matrix matWVP;

    float3 Pivot;
    float TransformEmpty1;

    float3 MeshSize;

	//ESCUnit_SBufferFlag 플래그 - 한 유닛이 '개별적으로' 특정 텍스처를 출력해야 할지 여부를 저장
    unsigned int SCUnit_SBufferFlag;

	//텍스처의 출력 부분은 구조화 버퍼에 저장
    Animation2DFrameData SCUnit_SBufferTexFrameInfo[(int)ESCUnit_TextureLayer_End];
};


#define SCUnitShadowRender 1 << 0

#define SCUnitMainRender 1 << 5
#define SCUnitMainXFlip 1 << 6
#define SCUnitCloaked 1 << 7

#define SCUnitTopRender 1 << 10
#define SCUnitTopXFlip 1 << 11

#define SCUnitEffectRender 1 << 15
//#define SCUnitEffectXFlip 1 << 16

#define SCUnitBoosterRender 1 << 20
//#define SCUnitBoosterXFlip 1 << 21

#define SCUnitAttackRender 1 << 25
//#define SCUnitAttackXFlip 1 << 26



Texture2DArray g_UnitMainTextureArray : register(t1);
texture2D g_UnitTopTexture : register(t2);
texture2D g_UnitEffectTexture : register(t3);
texture2D g_UnitBoosterTexture : register(t4);
texture2D g_UnitAttackTexture : register(t5);

StructuredBuffer<SCUnit_SBuffer> g_UnitInfoArray : register(t6);



VS_OUTPUT_UV SCUnitVS(VS_INPUT_UV input)
{
	VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;
    
    float3 Pos = input.Pos - g_UnitInfoArray[input.InstanceID].Pivot * g_UnitInfoArray[input.InstanceID].MeshSize;
    
    output.Pos = mul(float4(Pos, 1.f), g_UnitInfoArray[input.InstanceID].matWVP);
    
    ////애니메이션 계산. 무조건 ArrayIndexed 텍스처를 사용했다고 가정하고 계산한다.
    //float2 Result = (float2) 0;
    
    ////무조건 AtlasIndexed임을 상정하고 계산한다.
    //if (g_UnitInfoArray[input.InstanceID].SCUnit_SBufferFlag & SCUnitMainXFlip)
    //{
    //    if (output.UV.x == 0.f)
    //        output.UV.x = g_Anim2DFrameEnd.x / g_Anim2DImageWidth;
    //    else
    //        output.UV.x = g_Anim2DFrameStart.x / g_Anim2DImageWidth;
    //}
    //else
    //{
    //    if (output.UV.x == 0.f)
    //        output.UV.x = g_Anim2DFrameStart.x / g_Anim2DImageWidth;
    //    else
    //        output.UV.x = g_Anim2DFrameEnd.x / g_Anim2DImageWidth;
    //}
        
    //if (output.UV.y == 0.f)
    //    output.UV.y = g_Anim2DFrameStart.y / g_Anim2DImageHeight;
    //else
    //    output.UV.y = g_Anim2DFrameEnd.y / g_Anim2DImageHeight;
    
    output.UV = input.UV;
    
	return output;
}

PS_OUTPUT_SINGLE SCUnitPS(VS_OUTPUT_UV input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

	float4 TextureColor = (float4) 0.f;
    
    TextureColor = g_UnitMainTextureArray.Sample(g_PointSmp, float3(input.UV, 0.f));
    
	//TextureColor = g_UnitMainTexture.Sample(g_PointSmp, input.UV);
    
	output.Color.rgb = TextureColor.rgb * g_MtrlBaseColor.rgb * input.Color.rgb;

    output.Color.a = TextureColor.a;
    
	return output;
}

