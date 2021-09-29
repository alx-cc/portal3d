#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)


extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;
extern int window_width;
extern int window_height;

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
 * Draw a line to the color buffer using a DDA (digital differential analyzer) algorithm
 *
 * @param: x0 : starting point x value
 * @param: y0 : starting point y value
 * @param: x1 : ending point x value
 * @param: x2 : ending point y value
 * @param: color : color to draw line in
 */
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);

/**
 * Draw triangle to the color buffer (calls draw_line)
 */
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

/**
 * Just a test function to draw a grid to the color buffer, will prob delete this
 *
 * @param  color1: color of grid border
 * @param  color2: color of background
 */
void draw_grid(uint32_t color1, uint32_t color2);

void draw_horizon();

void destroy_window(void);
#endif

