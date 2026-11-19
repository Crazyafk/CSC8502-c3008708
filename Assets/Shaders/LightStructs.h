#pragma once
//type enum - matching Light.h
//0 is invalid
const int LightType_Point = 1;
const int LightType_Direction = 2;

struct LightData {
	int type;
	float dummyA;
	float dummyB;
	float dummyC;

	vec4 colour;

	//for the following, meaning is type dependent
	//
	// POINT: vec3_A is position, float_A is radius
	// DIRECTION: vec3_A is direction

	vec3 vec3_A; 
	float float_A;
};