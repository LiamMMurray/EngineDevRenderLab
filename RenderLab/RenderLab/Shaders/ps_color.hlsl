struct VSOut
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

struct PSOut
{
    float4 Color : SV_TARGET;
};

PSOut main(VSOut pin)
{
    PSOut pout;
    pout.Color = pin.Color;
	return pout;
}