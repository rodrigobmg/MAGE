//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#include "structures.fx"

float4 PS(PSInputPositionNormalTexture input) : SV_Target{
	const float c = 1.0f - saturate(length(input.p_view) / 5.0f);
	return float4(c, c, c, 1.0f);
}