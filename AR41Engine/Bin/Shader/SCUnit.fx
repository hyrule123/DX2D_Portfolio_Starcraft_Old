
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



	//공격/부스터 부분
    float AttackWidth;
    float AttackHeight;
    float2 AttackEmpty;

    float2 AttackStart;
    float2 AttackEnd;


	//공격/부스터 부분
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
	
    
    // mul : 행렬 곱. g_matWVP 는 World * View * Proj 이므로 정점을 여기에 곱하게 되면
    // 투영 공간으로 변환된 정점의 위치가 나온다.
    // 투영의 w에는 뷰공간의 z값이 들어온다.
    // 정점정보를 모두 계산해서 넘겨주고 픽셀출력을 하기 전에 먼저 투영공간의 x, y, z, w를
    // 투영공간의 w로 모두 나누어준다.
    // 이렇게 해주는 이유는 멀리 있는 물체는 작게, 가까이 있는 물체는 크게 표현이 되야 하는데
    // 투영공간의 w에는 뷰공간의 z가 들어가므로 이 값은 물체가 카메라로부터 떨어져 있는 거리를
    // 의미하므로 이 값으로 나누어주게 된다면 거리가 멀면 멀수록 큰 값으로 나누어주게 되기 때문에
    // 결과적으로 작은 값이 나오게 되어 물체가 작게 출력이 되는 방식이다.
    output.Pos = mul(float4(input.Pos, 1.f), g_UnitInfoArray[input.InstanceID].matWVP);
    
    
    //애니메이션 계산. 무조건 ArrayIndexed 텍스처를 사용했다고 가정하고 계산한다.
    float2 Result = (float2) 0;
    
    //무조건 AtlasIndexed임을 상정하고 계산한다.
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

