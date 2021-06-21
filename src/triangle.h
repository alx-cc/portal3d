#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"

//face_t stores indices of vertices (corner 1, 2, 3)
typedef struct {
	int a;
	int b;
	int c;
} face_t;

//triangle_t stores actual screen coords of each index (corner) ( e.g. (100,300), (360,100), (250, 400) )
typedef struct {
	vec2_t points[3];
} triangle_t;

#endif

