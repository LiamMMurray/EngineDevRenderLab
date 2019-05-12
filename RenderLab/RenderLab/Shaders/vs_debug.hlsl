cbuffer MVP_t
{
    matrix modeling;
    matrix view;
    matrix projection;
};

struct VSIn
{
    float3 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uvs : TEXCOORD;
};

struct VSOut
{
    float4 pos : SV_POSITION;
    float4 normal : NORMAL;
};


VSOut main(VSIn input)
{
    VSOut output;

    float4 vert_pos = float4(input.pos, 1.0f);
    float4 vert_norm = float4(input.normal, 0.0f);

    output.pos = mul(vert_pos, modeling);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.normal = mul(vert_norm, modeling);
    output.normal = mul(output.normal, view);

    return output;
}
	
