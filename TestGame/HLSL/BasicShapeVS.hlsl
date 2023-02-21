struct VS_IN
{
	float3 p : F3_POSITION;
	float3 n : F3_NORMAL;
	float4 c : F4_COLOR;
	float2 t : F2_TEXTURE;
	float4 i : F4_INDEX;
	float4 w : F4_WEIGHT;
};
struct VS_OUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	float4 vWorld : TEXCOORD1;
	float3 vLight : TEXCOORD2;
};

// ������۴� �������� �����θ� ����ȴ�.
// �������� ������ :  float4(x,y,z,w)

cbuffer cb_data : register(b0)
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView : packoffset(c4);
	matrix g_matProj : packoffset(c8);
};
VS_OUT VS(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	float4 vLocal = float4(input.p, 1.0f);

	// �߿�:  mul �Լ��� �������� ó���ȴ�.
	// vWolrd.x = vLocal dot c0; // ��*��
	// vWolrd.y = vLocal dot c1;
	// vWolrd.z = vLocal dot c2;
	// vWolrd.w = vLocal dot c3;	
	// matrix matTranspose = transpose(g_matWorld);

	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);

	output.p = vProj;
	output.c = input.c;

	return output;
}