struct PSInput
{
    float4 position : SV_POSITION; // Ekraani positsioon
    float4 color : COLOR; // Värv
};

float4 main(PSInput input) : SV_TARGET
{
    // Arvutage keskpunkt ja raadius ekraani mõõtmete põhjal
    float2 center = float2(0.0f, 0.0f); // Keskpunkt
    float radius = 50.0f; // Raadius (50 pixelit)

    // Arvuta kaugus keskpunktist
    float distance = length(input.position.xy - center);

    // Kui kaugus on väiksem kui raadius, joonista ring
    if (distance < radius)
    {
        return float4(1.0f, 0.0f, 0.0f, 1.0f); // Punane värv
    }
    return float4(0.0f, 0.0f, 0.0f, 1.0f); // Must värv
}