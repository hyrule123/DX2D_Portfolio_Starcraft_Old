
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



cbuffer SCUnitCBuffer : register(b12)
{
    unsigned int RenderFlags;
};

//플래그 값
#define UnitMainShadow = 1 << 0
#define UseShadowSprite = 1 << 1

//(있을 시)유닛의 상단 부분
#define UnitTop = 1 << 2

//유닛의 이펙트 부분
#define UnitEffect = 1 << 3
//유닛의 부스터 부분
#define UnitBooster = 1 << 4

//유닛의 공격
#define UnitAttack = 1 << 5




struct UnitInfo
{
    Matrix matWVP;

    float3 Pivot;
    float TransformEmpty1;

    float3 MeshSize;
	//플래그 - 한 유닛이 '개별적으로' 특정 텍스처를 출력해야 할지 여부를 저장
    unsigned int SCUnitSBufferFlag;


	//애니메이션 정보를 저장. 그림자와가 있는 스프라이트는 메인 유닛 몸체와 정확히 일치함.
	//그렇지 않을 경우 쉐이더를 통해서 해결을 봐야 할듯
    float MainShadowWidth;
    float MainShadowHeight;
    float2 MainShadowEmpty;

    float2 MainShadowStart;
    float2 MainShadowEnd;


	//상단 부분
    float TopWidth;
    float TopHeight;
    float2 TopEmpty;

    float2 TopStart;
    float2 TopEnd;


	//이펙트 부분
    float EffectWidth;
    float EffectHeight;
    float2 EffectEmpty;

    float2 EffectStart;
    float2 EffectEnd;

    //부스터 부분
    float BoosterWidth;
    float BoosterHeight;
    float2 BoosterEmpty;

    float2 BoosterStart;
    float2 BoosterEnd;

	//공격 부분
    float AttackWidth;
    float AttackHeight;
    float2 AttackEmpty;

    float2 AttackStart;
    float2 AttackEnd;
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



Texture2DArray g_UnitMainTexture : register(t1);
texture2D g_UnitTopTexture : register(t2);
texture2D g_UnitEffectTexture : register(t3);
texture2D g_UnitBoosterTexture : register(t4);
texture2D g_UnitAttackTexture : register(t5);

StructuredBuffer<UnitInfo> g_UnitInfoArray : register(t6);



VS_OUTPUT_UV SCUnitVS(VS_INPUT_UV input)
{
	VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;
	
    
    output.Pos = mul(float4(input.Pos, 1.f), g_UnitInfoArray[input.InstanceID].matWVP);
    
    
    //애니메이션 계산. 무조건 ArrayIndexed 텍스처를 사용했다고 가정하고 계산한다.
    float2 Result = (float2) 0;
    
    //무조건 AtlasIndexed임을 상정하고 계산한다.
    if (g_UnitInfoArray[input.InstanceID].SCUnitSBufferFlag & SCUnitMainXFlip)
    {
        if (output.UV.x == 0.f)
            output.UV.x = g_Anim2DFrameEnd.x / g_Anim2DImageWidth;
        else
            output.UV.x = g_Anim2DFrameStart.x / g_Anim2DImageWidth;
    }
    else
    {
        if (output.UV.x == 0.f)
            output.UV.x = g_Anim2DFrameStart.x / g_Anim2DImageWidth;
        else
            output.UV.x = g_Anim2DFrameEnd.x / g_Anim2DImageWidth;
    }
        
    if (output.UV.y == 0.f)
        output.UV.y = g_Anim2DFrameStart.y / g_Anim2DImageHeight;
    else
        output.UV.y = g_Anim2DFrameEnd.y / g_Anim2DImageHeight;
	return output;
}

PS_OUTPUT_SINGLE SCUnitPS(VS_OUTPUT_UV input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

	float4 TextureColor = (float4) 0.f;
    
	TextureColor = g_BaseTexture.Sample(g_PointSmp, input.UV);
    
    
	output.Color.rgb = TextureColor.rgb * g_MtrlBaseColor.rgb * input.Color.rgb;

    output.Color.a = TextureColor.a;
    
    
	return output;
}

