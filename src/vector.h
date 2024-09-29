#ifndef VECTOR_H
#define VECTOR_H

// a struct for 2D vectors containing each coordinate in the 2D plane
typedef struct {
  float x;
  float y;
} vec2_t;

// a struct for 3D vectors containing each coordinate in the 3D plane
typedef struct {
  float x;
  float y;
  float z;
} vec3_t;

// a struct for 4D vectors (needed for 4x4 matrices which are needed for linear
// transformations)
typedef struct {
  float x;
  float y;
  float z;
  float w;
} vec4_t;

// 2D vector functions
vec2_t vec2_new(float x, float y);
float vec2_length(vec2_t v);
vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_sub(vec2_t a, vec2_t b);
vec2_t vec2_mul(vec2_t v, float factor);
vec2_t vec2_div(vec2_t v, float factor);
void vec2_normalize(vec2_t *v);

// 3D vector functions
vec3_t vec3_new(float x, float y, float z);
vec3_t vec3_clone(vec3_t *v);
float vec3_length(vec3_t v);
vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_sub(vec3_t a, vec3_t b);
vec3_t vec3_mul(vec3_t v, float factor);
vec3_t vec3_div(vec3_t v, float factor);
vec3_t vec3_cross(vec3_t a, vec3_t b); // find the cross product of two vectors
float vec3_dot(vec3_t a, vec3_t b);
void vec3_normalize(vec3_t *v);

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);

// vector conversion functions
vec4_t vec4_from_vec3(vec3_t v);
vec3_t vec3_from_vec4(vec4_t v);
vec2_t vec2_from_vec4(vec4_t v);

#endif
