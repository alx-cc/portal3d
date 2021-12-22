#include <stdint.h>
#include <stdio.h>
#include "texture.h"

int texture_width = 64;
int texture_height = 64;

upng_t* png_texture = NULL;
uint32_t* mesh_texture = NULL;

void load_png_texture_data(char* filename) {
    png_texture = upng_new_from_file(filename); // pass .png file data into png_texture using uPNG lib
    // as long as the texture isn't NULL
    if (png_texture != NULL) {
        // decode all of the texture data from the png using uPNG lib
        upng_decode(png_texture);
        // if there were no errors in decoding...
        if (upng_get_error(png_texture) == UPNG_EOK) {
            // pass all of the needed .png image data in
            mesh_texture = (uint32_t*)upng_get_buffer(png_texture);
            // set our texture dimensions to what the png is in case our texture dimensions differ from
            // those of the png
            texture_width = upng_get_width(png_texture);
            texture_height = upng_get_height(png_texture);
        }
    }
}

tex2_t tex2_clone(tex2_t* t) {
    tex2_t result = { t->u, t-> v };
    return result;
}

