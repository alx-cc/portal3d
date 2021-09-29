#include <math.h>
#include "vector.h"

/**
* Get the sum of two 2D vectors
*/
vec2_t vec2_add(vec2_t a, vec2_t b) {
	vec2_t result = {
		.x = a.x + b.x,
		.y = a.y + b.y
	};
	return result;
}

/**
* Get the difference of two 2D vectors
*/
vec2_t vec2_sub(vec2_t a, vec2_t b) {
	vec2_t result = {
		.x = a.x - b.x,
		.y = a.y - b.y
	};
	return result;
}

/**
* Multiply a 2D vector by some scalar multiple
*/
vec2_t vec2_mul(vec2_t a, float factor) {
	vec2_t result = {
		.x = a.x * factor,
		.y = a.y * factor
	};
	return result;
}

/**
* Divide a 2D vector by some scalar multiple
*/
vec2_t vec2_div(vec2_t a, float factor) {
	vec2_t result = {
		.x = a.x / factor,
		.y = a.y / factor
	};
	return result;
}

/**
* Get the dot product of two 2D vectors using the formula (Ay*By + Ax+Bx)
*/
float vec2_dot(vec3_t a, vec2_t b) {
	return (a.x * b.x) + (a.y * b.y);
}

/**
* Normalize a 2d vector
*/
void vec2_normalize(vec2_t* v) {
    float length = sqrt(v->x * v->x + v->y * v->y);
    v->x /= length;
    v->y /= length;
}
/**
* Get the sum of two 3D vectors
*/
vec3_t vec3_add(vec3_t a, vec3_t b) {
	vec3_t result = {
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z
	};
	return result;
}

/**
* Get the difference of two 3D vectors
*/
vec3_t vec3_sub(vec3_t a, vec3_t b) {
	vec3_t result = {
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z
	};
	return result;
}

/**
* Multiply a 3D vector by some scalar multiple
*/
vec3_t vec3_mul(vec3_t a, float factor) {
	vec3_t result = {
		.x = a.x * factor,
		.y = a.y * factor,
		.z = a.z * factor
	};
	return result;
}

/**
* Divide a 3D vector by some scalar multiple
*/
vec3_t vec3_div(vec3_t a, float factor) {
	vec3_t result = {
		.x = a.x / factor,
		.y = a.y / factor,
		.z = a.z / factor
	};
	return result;
}

/**
* Get the cross product of two 3D vectors using the 3D vector cross product formula
*/
vec3_t vec3_cross(vec3_t a, vec3_t b) {
	vec3_t result = {
		.x = a.y * b.z - a.z * b.y,
		.y = a.z * b.x - a.x * b.z,
		.z = a.x * b.y - a.y * b.x
	};
	return result;
}

/**
* Get the dot product of two 3D vectors using the formula (Ax*Bx + Ay*By + Az*Bz)
*/
float vec3_dot(vec3_t a, vec3_t b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}



void vec3_normalize(vec3_t* v) {
    float length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x /= length;
    v->y /= length;
    v->z /= length;
}
/**
* Rotate a given vector about the x axis
*/
vec3_t vec3_rotate_x(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		.x = v.x,
		.y = v.y * cos(angle) - v.z * sin(angle),
		.z = v.y * sin(angle) + v.z * cos(angle)
	};
	return rotated_vector;
}

/**
* Rotate a given vector about the y axis
*/
vec3_t vec3_rotate_y(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		.x = v.x * cos(angle) + v.z * sin(angle),
		.y = v.y,
		.z = -v.x * sin(angle) + v.z * cos(angle)
	};
	return rotated_vector;
}

/**
* Rotate a given vector about the z axis
*/
vec3_t vec3_rotate_z(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		.x = v.x * cos(angle) - v.y * sin(angle),
		.y = v.x * sin(angle) + v.y * cos(angle),
		.z = v.z
	};
	return rotated_vector;
}

// VECTOR CONVERSION FUNCTIONS

/**
* COnvert a 3D vector into a 4D vector (with normalized 4th component)
*/
vec4_t vec4_from_vec3(vec3_t v) {
    vec4_t result = { v.x, v.y, v.z, 1.0 };
    return result;
}

vec3_t vec3_from_vec4(vec4_t v) {
    vec3_t result = { v.x, v.y, v.z };
    return result;
}

