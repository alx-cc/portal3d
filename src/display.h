#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#define FPS 120
#define FRAME_TARGET_TIME (1000 / FPS)

enum cull_method { CULL_NONE, CULL_BACKFACE };

enum render_method {
  RENDER_WIRE,
  RENDER_WIRE_VERTEX,
  RENDER_FILL_TRIANGLE,
  RENDER_FILL_TRIANGLE_WIRE,
  RENDER_TEXTURED,
  RENDER_TEXTURED_WIRE
};

/**
 * get window dimensions
 */
int get_window_width(void);
int get_window_height(void);

/**
 * set render method (textured, wireframe, solid)
 */
void set_render_method(int method);

/**
 * enable or disable backface culling
 */
void set_cull_method(int method);

/**
 * check if backface culling is enabled
 */
bool is_cull_backface(void);

/**
 * Initialize SDL, initialize/configure the window we will be using
 * and initialize the renderer for that window
 *
 * @return boolean: indicate whether window opened succesfully or not
 */
bool initialize_window(void);

/**
 * Get the color buffer in memory and copy all of those pixel's values to
 * the texture so they can be displayed
 */
void render_color_buffer(void);

/**
 * Clear the color buffer (to be called before displaying a new frame)
 *
 * @param  color: color value to clear individual pixels with
 */
void clear_color_buffer(uint32_t color);

void clear_z_buffer(void);

float get_zbuffer_at(int x, int y);
void set_zbuffer_at(int x, int y, float value);

/**
 *
 */
void draw_pixel(int x, int y, uint32_t color);

/**
 * Draw a rectangle to the color buffer
 *
 * @param  xPos: x-coordinate of top-left position of rectangle
 * @param  yPos: y-coordinate of top-left position of rectangle
 * @param  width: pixel width of rectangle
 * @param  height: pixel height of rectangle
 * @param  color: color of rectangle
 */
void draw_rect(int xPos, int yPos, int width, int height, uint32_t color);

/**
 * Draw a line to the color buffer using a DDA (digital differential analyzer)
 * algorithm
 *
 * @param: x0 : starting point x value
 * @param: y0 : starting point y value
 * @param: x1 : ending point x value
 * @param: x2 : ending point y value
 * @param: color : color to draw line in
 */
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);

/**
 * Just a test function to draw a grid to the color buffer, will prob delete
 * this
 *
 * @param  color1: color of grid border
 * @param  color2: color of background
 */
void draw_grid(uint32_t color1, uint32_t color2);

void draw_horizon();

bool should_render_filled_triangles(void);
bool should_render_textured_triangles(void);
bool should_render_wireframe(void);
bool should_render_wire_vertex(void);

void destroy_window(void);
#endif
