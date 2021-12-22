#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>

#include "upng.h"

typedef struct {
    float u;
    float v;
} tex2_t;

extern int texture_width;
extern int texture_height;

extern const uint8_t REDBRICK_TEXTURE[];

extern uint32_t* mesh_texture;
// we need a upng_t instance to read in .png image data using uPNG
extern upng_t* png_texture;

// load .png image data
void load_png_texture_data(char* filename);

tex2_t tex2_clone(tex2_t* t);
#endif

