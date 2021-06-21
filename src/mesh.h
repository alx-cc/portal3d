#ifndef MESH_H
#define MESH_H

//USER-DEFINED INCLUDES
#include "vector.h"
#include "triangle.h"


//GLOBAL VARIABLES
#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) //6 cube faces, 2 triangles per face

extern face_t cube_faces[N_CUBE_FACES];
extern vec3_t cube_vertices[N_CUBE_VERTICES];

//define a struct for dynamically sized meshes with arrays of faces and vertices
typedef struct {
	vec3_t* vertices; //dynamic array of vertices
	face_t* faces; //dynamic array of faces
	vec3_t rotation; // rotation with x, y, and z values
} mesh_t;

extern mesh_t mesh; //our global mesh we'll use across the program

#endif

