#ifndef MAGE_HEADER_LIGHT
#define MAGE_HEADER_LIGHT

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#include "math.hlsli"

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------

/**
 A struct of directional lights.
 */
struct DirectionalLight {
	// The intensity of this spotlight .
	float3 I;
	uint padding0;
	// The (normalized) negated direction of this spotlight in camera-space coordinates.
	float3 neg_d;
	uint padding1;
};

/**
 A struct of omni lights.
 */
struct OmniLight {
	// The position of this omni light in camera-space coordinates.
	float3 p;
	//  The distance at which intensity falloff ends of this omni light.
	float distance_falloff_end;
	// The intensity of this omni light.
	float3 I;
	// The distance inverse range where intensity falloff occurs of this omni light.
	// distance_falloff_range     = distance_falloff_end - distance_falloff_start
	// distance_falloff_inv_range = 1 / distance_falloff_range
	float distance_falloff_inv_range;
};

/**
 A struct of spotlights.
 */
struct SpotLight {
	// The position of this spotlight in camera-space coordinates.
	float3 p;
	uint padding0;
	// The intensity of this spotlight .
	float3 I;
	uint padding1;
	// The (normalized) negated direction of this spotlight in camera-space coordinates.
	float3 neg_d;
	// The exponent property of this spotlight.
	float exponent_property;
	// The distance at which intensity falloff ends of this spotlight.
	float distance_falloff_end;
	// The distance inverse range where intensity falloff occurs of this spotlight.
	// distance_falloff_range     = distance_falloff_end - distance_falloff_start
	// distance_falloff_inv_range = 1 / distance_falloff_range
	float distance_falloff_inv_range;
	// The cosine of the umbra angle at which intensity falloff ends of this spotlight.
	float cos_umbra;
	// The cosine inverse range where intensity falloff occurs of this spotlight.
	// cos_range     = cos_penumbra - cos_umbra
	// cos_inv_range = 1 / cos_range
	float cos_inv_range;
};

/**
 A struct of directional lights with shadow mapping.
 */
struct DirectionalLightWithShadowMapping {
	// The directional light of this directional light with shadow mapping.
	DirectionalLight light;
	// The camera-view-to-light-projection transformation matrix.
	float4x4 cview_to_lprojection;
};

/**
 A struct of omni lights with shadow mapping.
 */
struct OmniLightWithShadowMapping {
	// The omni light of this omni light with shadow mapping.
	OmniLight light;
	// The camera-view-to-light-view transformation matrix.
	float4x4 cview_to_lview;
};

/**
 A struct of spotlights with shadow mapping.
 */
struct SpotLightWithShadowMapping {
	// The spotlight of this spotlight with shadow mapping.
	SpotLight light;
	// The camera-view-to-light-projection transformation matrix.
	float4x4 cview_to_lprojection;
};

/**
 Calculates the distance intensity fallof of a light.

 @param[in]		r
				The distance between the lit point and 
				the center of the light.
 @param[in]		r_end
				The distance at which intensity falloff ends 
				of the light.
 @param[in]		r_inv_range
				The distance inverse range where intensity falloff occurs 
				of the light.
 */
float DistanceFalloff(float r, float r_end, float r_inv_range) {
	return saturate((r_end - r) * r_inv_range);
}

/**
 Calculates the angular intensity fallof of a light.

 @param[in]		cos_theta
				The cosine of the angle between the direction
				from the center of the light to the lit point
				to the light direction.
 @param[in]		cos_umbra
				The cosine of the umbra angle at which intensity falloff 
				ends of the light.
 @param[in]		cos_inv_range
				The cosine inverse range where intensity falloff occurs 
				of the light.
 @param[in]		s_exp
				The exponent property of the light.
 */
float AngularFalloff(float cos_theta, float cos_umbra, float cos_inv_range, float s_exp) {
	return pow(saturate((cos_theta - cos_umbra) * cos_inv_range), s_exp);
}

/**
 Calculates the maximal intensity contribution of the given omni light.

 @param[in]		light
				The omni light.
 @param[in]		r
				The distance between the lit point and 
				the center of the light.
 */
float3 MaxContribution(OmniLight light, float r) {
	const float df = DistanceFalloff(r, light.distance_falloff_end, light.distance_falloff_inv_range);
	return df * light.I;
}

/**
 Calculates the maximal intensity contribution of the given spotlight.

 @pre			@a l is normalized.
 @param[in]		light
				The spotlight.
 @param[in]		r
				The distance between the lit point and 
				the center of the light.
 @param[in]		l
				The light (hit-to-light) direction.
 */
float3 MaxContribution(SpotLight light, float r, float3 l) {
	const float cos_theta = dot(light.neg_d, l);
	const float df = DistanceFalloff(r, light.distance_falloff_end, light.distance_falloff_inv_range);
	const float af = AngularFalloff(cos_theta, light.cos_umbra, light.cos_inv_range, light.exponent_property);
	return af * df * light.I;
}

void Contribution(DirectionalLight light, 
	out float3 l, out float3 I) {

	l = light.neg_d;
	I = light.I;
}

void Contribution(OmniLight light, 
	float3 p, out float3 l, out float3 I) {

	const float3 d_light     = light.p - p;
	const float  r_light     = length(d_light);
	const float  inv_r_light = 1.0f / r_light;
	l = d_light * inv_r_light;
	I = MaxContribution(light, r_light);
}

void Contribution(SpotLight light, 
	float3 p, out float3 l, out float3 I) {

	const float3 d_light     = light.p - p;
	const float  r_light     = length(d_light);
	const float  inv_r_light = 1.0f / r_light;
	l = d_light * inv_r_light;
	I = MaxContribution(light, r_light, l);
}

float ShadowFactor(SamplerComparisonState pcf_sampler,
	Texture2DArray shadow_maps, uint index, float4 p_proj) {

	const float  inv_w  = 1.0f / p_proj.w;
	const float3 p_ndc  = p_proj.xyz * inv_w;
	const float3 loc    = float3(NDCtoUV(p_ndc.xy), index);
	return shadow_maps.SampleCmpLevelZero(pcf_sampler, loc, p_ndc.z);
}

float ShadowFactor(SamplerComparisonState pcf_sampler, 
	TextureCubeArray shadow_maps, uint index, float3 p_view) {

	const float  r     = length(p_view);
	const float  inv_r = 1.0f / r;
	const float4 loc   = float4(p_view * inv_r, index);
	return shadow_maps.SampleCmpLevelZero(pcf_sampler, loc, r);
}

void Contribution(DirectionalLightWithShadowMapping light,
	SamplerComparisonState pcf_sampler, 
	Texture2DArray shadow_maps, uint index,
	float3 p, out float3 l, out float3 I) {

	float3 l0, I0;
	Contribution(light.light, l0, I0);

	l = l0;
	const float4 p_proj = mul(float4(p, 1.0f), light.cview_to_lprojection);
	I = I0 * ShadowFactor(pcf_sampler, shadow_maps, index, p_proj);
}

void Contribution(OmniLightWithShadowMapping light,
	SamplerComparisonState pcf_sampler, 
	TextureCubeArray shadow_maps, uint index,
	float3 p, out float3 l, out float3 I) {

	float3 l0, I0;
	Contribution(light.light, p, l0, I0);

	l = l0;
	const float3 p_view = mul(float4(p, 1.0f), light.cview_to_lview).xyz;
	I = ShadowFactor(pcf_sampler, shadow_maps, index, p_view);
}

void Contribution(SpotLightWithShadowMapping light,
	SamplerComparisonState pcf_sampler,
	Texture2DArray shadow_maps, uint index,
	float3 p, out float3 l, out float3 I) {

	float3 l0, I0;
	Contribution(light.light, p, l0, I0);

	l = l0;
	const float4 p_proj = mul(float4(p, 1.0f), light.cview_to_lprojection);
	I = I0 * ShadowFactor(pcf_sampler, shadow_maps, index, p_proj);
}

#endif //MAGE_HEADER_LIGHT