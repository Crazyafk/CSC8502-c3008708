
//type enum - matching Light.h
const int LightType_Point = 0;
const int LightType_Direction = 1;

struct LightData {
	int type;
	vec4 colour;

	//for the following, meaning is type dependent
	//
	// POINT: vec3_A is position, float_A is radius
	// DIRECTION: vec3_A is direction

	vec3 vec3_A; 
	float float_A;
};