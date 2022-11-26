
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
	matrix matWVP;
	float2 Start;
	float2 End;
	float4 TypeColor;
	float Opacity;
	int AnimationType;
	int Frame;
	float Empty;
};

cbuffer TileMapCBuffer : register(b12)
{
    float2 g_TileImageSize;
    float2 g_TileStart;
    float2 g_TileEnd;
    float2 g_TileSize;
    matrix g_TilematWVP;
    int g_TileFrame;
    float3 g_TileEmpty;
};

texture2D g_UnitShadTexture : register(t2);
texture2D g_UnitBackTexture : register(t3);
texture2D g_UnitFrontTexture : register(t4);
texture2D g_UnitBoosterTexture : register(t5);
texture2D g_UnitEffectTexture : register(t6);
 
StructuredBuffer<UnitInfo> g_UnitInfoArray : register(t8);

float2 UpdateTileAnimation2D(float2 UV, int InstanceID)
{
	float2 Result = (float2) 0;
    
	if (g_MtrlTextureType == Anim2D_Atlas)
	{
		if (UV.x == 0.f)
            Result.x = g_UnitInfoArray[InstanceID].Start.x / g_MtrlTextureWidth;
		else
            Result.x = g_UnitInfoArray[InstanceID].End.x / g_MtrlTextureWidth;
    
		if (UV.y == 0.f)
            Result.y = g_UnitInfoArray[InstanceID].Start.y / g_MtrlTextureHeight;
		else
            Result.y = g_UnitInfoArray[InstanceID].End.y / g_MtrlTextureHeight;
    }
	else
		Result = UV;
        
	return Result;
}


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
    output.Pos = mul(float4(input.Pos, 1.f), g_UnitInfoArray[input.InstanceID].matWVP).xyww;
    
	output.UV = UpdateTileAnimation2D(input.UV, input.InstanceID);
    
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

