#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>
#include "texture.h"
#include "vector.h"

//face_t stores indices of vertices (corner 1, 2, 3)
typedef struct {
	int a;
	int b;
	int c;
    tex2_t a_uv;
    tex2_t b_uv;
    tex2_t c_uv;
    uint32_t color;
} face_t;

//triangle_t stores actual screen coords of each index (corner) ( e.g. (100,300), (360,100), (250, 400) )
typedef struct {
	vec4_t points[3];
    tex2_t texcoords[3];
    uint32_t color;
} triangle_t;

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void draw_filled_triangle(
    int x0, int y0, float z0, float w0,
    int x1, int y1, float z1, float w1,
    int x2, int y2, float z2, float w2,
    uint32_t color
);
void draw_texel(
    int x, int y, uint32_t* texture,
    vec4_t point_a, vec4_t point_b, vec4_t point_c,
    tex2_t a_uv, tex2_t b_uv, tex2_t c_uv
);
// AFFINE MAPPING (draw_texel):
/*
void draw_texel(
    int x, int y, uint32_t* texture,
    vec2_t point_a, vec2_t point_b, vec2_t point_c,
    float u0, float v0, float u1, float v1, float u2, float v2
);
*/
// AFFINE MAPPING:
/*
void draw_textured_triangle(
        int x0, int y0, float u0, float v0,
        int x1, int y1, float u1, float v1,
        int x2, int y2, float u2, float v2,
        uint32_t* texture
);
*/

void draw_textured_triangle(
        int x0, int y0, float z0, float w0, float u0, float v0,
        int x1, int y1, float z1, float w1, float u1, float v1,
        int x2, int y2, float z2, float w2, float u2, float v2,
        uint32_t* texture
);

#endif

