#ifndef MESH_H
#define MESH_H

//USER-DEFINED INCLUDES
#include "vector.h"
#include "triangle.h"
#include "upng.h"

//define a struct for dynamically sized meshes with arrays of faces and vertices
typedef struct {
	vec3_t* vertices; 		// dynamic array of vertices
	face_t* faces; 				// dynamic array of faces
	upng_t* texture; 			// pointer to mesh PNG texture
	vec3_t rotation; 			// rotation with x, y, and z values
    vec3_t scale; 			// scale with x, y and z values
    vec3_t translation; // translate with x, y and z values
} mesh_t;

void load_mesh(char* obj_filename, char* png_filename, vec3_t scale, vec3_t translation, vec3_t rotation);
void load_mesh_obj_data(mesh_t* mesh, char* obj_filename);
void load_mesh_png_data(mesh_t* mesh, char* png_filename);

int get_num_meshes(void);
mesh_t* get_mesh(int index);

void free_meshes(void);
#endif
