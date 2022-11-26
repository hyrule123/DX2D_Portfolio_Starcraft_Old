
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
	float2 OriginUV : TEXCOORD1;
	float4 Color : COLOR;
	float Opacity : TEXCOORD2;
	int TextureType : TEXCOORD3;
	int Frame : TEXCOORD4;
};



struct UnitInfo
{
    Matrix matWVP;

    float3 Pivot;
    float TransformEmpty1;

    float3 MeshSize;
	//�÷��� - �� ������ '����������' Ư�� �ؽ�ó�� ����ؾ� ���� ���θ� ����
    unsigned int SCUnitSBufferFlag;


	//�ִϸ��̼� ������ ����. �׸��ڿͰ� �ִ� ��������Ʈ�� ���� ���� ��ü�� ��Ȯ�� ��ġ��.
	//�׷��� ���� ��� ���̴��� ���ؼ� �ذ��� ���� �ҵ�
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



	//����/�ν��� �κ�
    float AttackWidth;
    float AttackHeight;
    float2 AttackEmpty;

    float2 AttackStart;
    float2 AttackEnd;


	//����/�ν��� �κ�
    float BoosterWidth;
    float BoosterHeight;
    float2 BoosterEmpty;

    float2 BoosterStart;
    float2 BoosterEnd;

};

cbuffer SCUnitCBuffer : register(b12)
{
    unsigned int RenderFlags;
};

texture2D g_UnitShadTexture : register(t2);
texture2D g_UnitBackTexture : register(t3);
texture2D g_UnitFrontTexture : register(t4);
texture2D g_UnitBoosterAttackTexture : register(t5);
texture2D g_UnitEffectTexture : register(t6);

StructuredBuffer<UnitInfo> g_UnitInfoArray : register(t8);



VS_OUTPUT_UV SCUnitVS(VS_INPUT_UV input)
{
	VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;
	
    
    // mul : ��� ��. g_matWVP �� World * View * Proj �̹Ƿ� ������ ���⿡ ���ϰ� �Ǹ�
    // ���� �������� ��ȯ�� ������ ��ġ�� ���´�.
    // ������ w���� ������� z���� ���´�.
    // ���������� ��� ����ؼ� �Ѱ��ְ� �ȼ������ �ϱ� ���� ���� ���������� x, y, z, w��
    // ���������� w�� ��� �������ش�.
    // �̷��� ���ִ� ������ �ָ� �ִ� ��ü�� �۰�, ������ �ִ� ��ü�� ũ�� ǥ���� �Ǿ� �ϴµ�
    // ���������� w���� ������� z�� ���Ƿ� �� ���� ��ü�� ī�޶�κ��� ������ �ִ� �Ÿ���
    // �ǹ��ϹǷ� �� ������ �������ְ� �ȴٸ� �Ÿ��� �ָ� �ּ��� ū ������ �������ְ� �Ǳ� ������
    // ��������� ���� ���� ������ �Ǿ� ��ü�� �۰� ����� �Ǵ� ����̴�.
    output.Pos = mul(float4(input.Pos, 1.f), g_UnitInfoArray[input.InstanceID].matWVP);
    
    
    //�ִϸ��̼� ���. ������ ArrayIndexed �ؽ�ó�� ����ߴٰ� �����ϰ� ����Ѵ�.
    float2 Result = (float2) 0;
    
    //������ AtlasIndexed���� �����ϰ� ����Ѵ�.
    if (g_UnitInfoArray[input.InstanceID] == 1)
    {
        if (UV.x == 0.f)
            Result.x = g_Anim2DFrameEnd.x / g_Anim2DImageWidth;
        else
            Result.x = g_Anim2DFrameStart.x / g_Anim2DImageWidth;
   
    }
    else
    {
        if (UV.x == 0.f)
            Result.x = g_Anim2DFrameStart.x / g_Anim2DImageWidth;
        else
            Result.x = g_Anim2DFrameEnd.x / g_Anim2DImageWidth;
    }
        
    if (UV.y == 0.f)
        Result.y = g_Anim2DFrameStart.y / g_Anim2DImageHeight;
    else
        Result.y = g_Anim2DFrameEnd.y / g_Anim2DImageHeight;


	

    
	output.OriginUV = input.UV;
    output.Color = g_UnitInfoArray[input.InstanceID].TypeColor;
    output.Opacity = g_UnitInfoArray[input.InstanceID].Opacity;
	output.TextureType = g_MtrlTextureType;
    output.Frame = g_UnitInfoArray[input.InstanceID].Frame;
    
	return output;
}

PS_OUTPUT_SINGLE SCUnitPS(VS_OUTPUT_UV input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

	float4 TextureColor = (float4) 0.f;
    
	if (input.TextureType != 2)
		TextureColor = g_BaseTexture.Sample(g_PointSmp, input.UV);
    
    
	output.Color.rgb = TextureColor.rgb * g_MtrlBaseColor.rgb * input.Color.rgb;

	output.Color.a = TextureColor.a * input.Opacity;
    
    
	return output;
}

