
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
    
    // mul : 행렬 곱. g_matWVP 는 World * View * Proj 이므로 정점을 여기에 곱하게 되면
    // 투영 공간으로 변환된 정점의 위치가 나온다.
    // 투영의 w에는 뷰공간의 z값이 들어온다.
    // 정점정보를 모두 계산해서 넘겨주고 픽셀출력을 하기 전에 먼저 투영공간의 x, y, z, w를
    // 투영공간의 w로 모두 나누어준다.
    // 이렇게 해주는 이유는 멀리 있는 물체는 작게, 가까이 있는 물체는 크게 표현이 되야 하는데
    // 투영공간의 w에는 뷰공간의 z가 들어가므로 이 값은 물체가 카메라로부터 떨어져 있는 거리를
    // 의미하므로 이 값으로 나누어주게 된다면 거리가 멀면 멀수록 큰 값으로 나누어주게 되기 때문에
    // 결과적으로 작은 값이 나오게 되어 물체가 작게 출력이 되는 방식이다.
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

