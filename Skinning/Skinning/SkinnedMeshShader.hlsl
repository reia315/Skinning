cbuffer ConstantBuffer : register(b0)
{
	matrix      WorldMatrix;
	matrix      ViewMatrix;
	matrix      ProjectionMatrix;
	matrix      BoneMatrices[256];
};

struct VS_INPUT
{
	float4  Position     : POSITION;
	float3  Normal       : NORMAL;
	float2  TexCoord     : TEXCOORD;
	int4    BlendIndices : BLENDINDICES;
	float4  BlendWeight  : BLENDWEIGHT;
};

struct VS_OUTPUT
{
	float4 Position      : SV_POSITION;
	float2 TexCoord      : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT Input)
{
	VS_OUTPUT Output;

	matrix LocalMatrix = BoneMatrices[Input.BlendIndices.x] * Input.BlendWeight.x
		+ BoneMatrices[Input.BlendIndices.y] * Input.BlendWeight.y
		+ BoneMatrices[Input.BlendIndices.z] * Input.BlendWeight.z
		+ BoneMatrices[Input.BlendIndices.w] * Input.BlendWeight.w;

	float4 LocalPosition = mul(LocalMatrix, Input.Position);
	float4 WorldPosition = mul(WorldMatrix, LocalPosition);
	float4 ViewPosition = mul(ViewMatrix, WorldPosition);

	Output.Position = mul(ProjectionMatrix, ViewPosition);
	Output.TexCoord = Input.TexCoord;

	return Output;
}

SamplerState g_AlbedoMapSampler  : register(s0);
Texture2D    g_AlbedoMapTexture  : register(t0);

float4 PS(VS_OUTPUT Input) : SV_Target
{
	float4 color = g_AlbedoMapTexture.Sample(g_AlbedoMapSampler, Input.TexCoord);
	return color;
}