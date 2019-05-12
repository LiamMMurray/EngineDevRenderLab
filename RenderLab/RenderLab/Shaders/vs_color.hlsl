cbuffer MVP_t
{
    matrix modeling;
    matrix view;
    matrix projection;
};

struct VSIn
{
    float3 Position : SV_Position;
    float4 Color : COLOR;
};

struct VSOut
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

VSOut main(VSIn vin)
{
    VSOut vout;
    vout.Position = mul(float4(vin.Position.x, vin.Position.y, vin.Position.z, 1.f), modeling);
    vout.Position = mul(vout.Position, view);
    vout.Position = mul(vout.Position, projection);
    vout.Color = vin.Color;
	return vout;
}