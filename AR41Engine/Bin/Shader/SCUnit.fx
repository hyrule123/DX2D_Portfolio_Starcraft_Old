
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

//�÷��� ��
#define UnitMainShadow = 1 << 0
#define UseShadowSprite = 1 << 1

//(���� ��)������ ��� �κ�
#define UnitTop = 1 << 2

//������ ����Ʈ �κ�
#define UnitEffect = 1 << 3
//������ �ν��� �κ�
#define UnitBooster = 1 << 4

//������ ����
#define UnitAttack = 1 << 5




struct UnitInfo
{
    Matrix matWVP;

    float3 Pivot;
    float TransformEmpty1;

    float3 MeshSize;

	//ESCUnit_SBufferFlag �÷��� - �� ������ '����������' Ư�� �ؽ�ó�� ����ؾ� ���� ���θ� ����
    unsigned int SCUnit_SBufferFlag;

    float SelectedWidth;
    float SelectedHeight;
    float2 SelectedEmpty;
    float2 SelectedStart;
    float2 SelectedEnd;

	//�ִϸ��̼� ������ ����. �׸��ڰ� �ִ� ��������Ʈ�� ���� ���� ��ü�� ��Ȯ�� ��ġ��.
	//�׸��� ��������Ʈ�� ���� ���µ� �׸��ڸ� �������ϴ� ���� ��������Ʈ�� ���� ��ü ��������Ʈ�� �״�� �Ʒ��� �׷����� ������ ��������.
    float MainShadowWidth;
    float MainShadowHeight;
    float2 MainShadowEmpty;

    float2 MainShadowStart;
    float2 MainShadowEnd;


	//��� �κ�
    float TopWidth;
    float TopHeight;
    float2 TopEmpty;

    float2 TopStart;
    float2 TopEnd;


	//����Ʈ �κ�
    float EffectWidth;
    float EffectHeight;
    float2 EffectEmpty;

    float2 EffectStart;
    float2 EffectEnd;


	//�ν���/���� �κ�
    float AttackBoosterWidth;
    float AttackBoosterHeight;
    float2 AttackBoosterEmpty;

    float2 AttackBoosterStart;
    float2 AttackBoosterEnd;
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
    
    
    //�ִϸ��̼� ���. ������ ArrayIndexed �ؽ�ó�� ����ߴٰ� �����ϰ� ����Ѵ�.
    float2 Result = (float2) 0;
    
    //������ AtlasIndexed���� �����ϰ� ����Ѵ�.
    if (g_UnitInfoArray[input.InstanceID].SCUnit_SBufferFlag & SCUnitMainXFlip)
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

