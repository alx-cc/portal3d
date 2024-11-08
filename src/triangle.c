#include "triangle.h"
#include "display.h"
#include "swap.h"

/**
 * Return the barycentric weights alpha, beta, and gamma for point p
 **/
///////////////////////////////////////////////////////////////////////////////
//
//          A
//         /|\
//        / | \
//       /  |  \
//      /  (p)  \
//     /  /   \  \
//    / /       \ \
//   B-------------C
//
///////////////////////////////////////////////////////////////////////////////
vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p) {
  // Find the vectors between the vertices ABC and point p
  vec2_t ab = vec2_sub(b, a);
  vec2_t bc = vec2_sub(c, b);
  vec2_t ac = vec2_sub(c, a);
  vec2_t ap = vec2_sub(p, a);
  vec2_t bp = vec2_sub(p, b);

  // Calcualte the area of the full triangle ABC using cross product (area of
  // parallelogram)
  float area_triangle_abc = (ab.x * ac.y - ab.y * ac.x);

  // Weight alpha is the area of subtriangle BCP divided by the area of the full
  // triangle ABC
  float alpha = (bc.x * bp.y - bp.x * bc.y) / area_triangle_abc;

  // Weight beta is the area of subtriangle ACP divided by the area of the full
  // triangle ABC
  float beta = (ap.x * ac.y - ac.x * ap.y) / area_triangle_abc;

  // Weight gamma is easily found since barycentric cooordinates always add up
  // to 1
  float gamma = 1 - alpha - beta;

  vec3_t weights = {alpha, beta, gamma};
  return weights;
}

///////////////////////////////////////////////////////////////////////////////
// Draw a triangle using three raw line calls
///////////////////////////////////////////////////////////////////////////////
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                   uint32_t color) {
  draw_line(x0, y0, x1, y1, color);
  draw_line(x1, y1, x2, y2, color);
  draw_line(x2, y2, x0, y0, color);
}

///////////////////////////////////////////////////////////////////////////////
// Function to draw a solid pixel at position (x,y) using depth interpolation
///////////////////////////////////////////////////////////////////////////////
void draw_triangle_pixel(int x, int y, uint32_t color, vec4_t point_a,
                         vec4_t point_b, vec4_t point_c) {
  // Create three vec2 to find the interpolation
  vec2_t p = {x, y};
  vec2_t a = vec2_from_vec4(point_a);
  vec2_t b = vec2_from_vec4(point_b);
  vec2_t c = vec2_from_vec4(point_c);

  // Calculate the barycentric coordinates of our point 'p' inside the triangle
  vec3_t weights = barycentric_weights(a, b, c, p);

  float alpha = weights.x;
  float beta = weights.y;
  float gamma = weights.z;

  // Interpolate the value of 1/w for the current pixel
  float interpolated_reciprocal_w = (1 / point_a.w) * alpha +
                                    (1 / point_b.w) * beta +
                                    (1 / point_c.w) * gamma;

  // Adjust 1/w so the pixels that are closer to the camera have smaller values
  interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

  // Only draw the pixel if the depth value is less than the one previously
  // stored in the z-buffer
  if (interpolated_reciprocal_w < get_zbuffer_at(x, y)) {
    // Draw a pixel at position (x,y) with a solid color
    draw_pixel(x, y, color);

    // Update the z-buffer value with the 1/w of this current pixel
    set_zbuffer_at(x, y, interpolated_reciprocal_w);
  }
}

void draw_filled_triangle(int x0, int y0, float z0, float w0, int x1, int y1,
                          float z1, float w1, int x2, int y2, float z2,
                          float w2, uint32_t color) {
  // We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
  if (y0 > y1) {
    int_swap(&y0, &y1);
    int_swap(&x0, &x1);
    float_swap(&z0, &z1);
    float_swap(&w0, &w1);
  }
  if (y1 > y2) {
    int_swap(&y1, &y2);
    int_swap(&x1, &x2);
    float_swap(&z1, &z2);
    float_swap(&w1, &w2);
  }
  if (y0 > y1) {
    int_swap(&y0, &y1);
    int_swap(&x0, &x1);
    float_swap(&z0, &z1);
    float_swap(&w0, &w1);
  }

  // Create three vector points after we sort the vertices
  vec4_t point_a = {x0, y0, z0, w0};
  vec4_t point_b = {x1, y1, z1, w1};
  vec4_t point_c = {x2, y2, z2, w2};

  ///////////////////////////////////////////////////////
  // Render the upper part of the triangle (flat-bottom)
  ///////////////////////////////////////////////////////
  float inv_slope_1 = 0;
  float inv_slope_2 = 0;

  if (y1 - y0 != 0)
    inv_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
  if (y2 - y0 != 0)
    inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

  if (y1 - y0 != 0) {
    for (int y = y0; y <= y1; y++) {
      int x_start = x1 + (y - y1) * inv_slope_1;
      int x_end = x0 + (y - y0) * inv_slope_2;

      if (x_end < x_start) {
        int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
      }

      for (int x = x_start; x < x_end; x++) {
        // Draw our pixel with a solid color
        draw_triangle_pixel(x, y, color, point_a, point_b, point_c);
      }
    }
  }

  ///////////////////////////////////////////////////////
  // Render the bottom part of the triangle (flat-top)
  ///////////////////////////////////////////////////////
  inv_slope_1 = 0;
  inv_slope_2 = 0;

  if (y2 - y1 != 0)
    inv_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
  if (y2 - y0 != 0)
    inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

  if (y2 - y1 != 0) {
    for (int y = y1; y <= y2; y++) {
      int x_start = x1 + (y - y1) * inv_slope_1;
      int x_end = x0 + (y - y0) * inv_slope_2;

      if (x_end < x_start) {
        int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
      }

      for (int x = x_start; x < x_end; x++) {
        // Draw our pixel with a solid color
        draw_triangle_pixel(x, y, color, point_a, point_b, point_c);
      }
    }
  }
}

/**
 * Draw the textured pixel at position x and y using interpolation
 **/
void draw_texel(int x, int y, upng_t *texture, vec4_t point_a, vec4_t point_b,
                vec4_t point_c, tex2_t a_uv, tex2_t b_uv, tex2_t c_uv) {
  vec2_t p = {x, y};
  vec2_t a = vec2_from_vec4(point_a);
  vec2_t b = vec2_from_vec4(point_b);
  vec2_t c = vec2_from_vec4(point_c);

  vec3_t weights = barycentric_weights(a, b, c, p);

  float alpha = weights.x;
  float beta = weights.y;
  float gamma = weights.z;

  // Variables to store the interpolated values of U, V, and also 1/w for the
  // current pixel
  float interpolated_u;
  float interpolated_v;
  float interpolated_reciprocal_w;

  // Perform the interpolation of all U/w and V/w values using barycentric
  // weights and a factor of 1/w
  interpolated_u = (a_uv.u / point_a.w) * alpha + (b_uv.u / point_b.w) * beta +
                   (c_uv.u / point_c.w) * gamma;
  interpolated_v = (a_uv.v / point_a.w) * alpha + (b_uv.v / point_b.w) * beta +
                   (c_uv.v / point_c.w) * gamma;

  // Also interpolate the value of 1/w for the current pixel
  interpolated_reciprocal_w = (1 / point_a.w) * alpha + (1 / point_b.w) * beta +
                              (1 / point_c.w) * gamma;

  // Now we can divide back both interpolated values by 1/w
  interpolated_u /= interpolated_reciprocal_w;
  interpolated_v /= interpolated_reciprocal_w;

  // get texture dimenions
  int texture_width = upng_get_width(texture);
  int texture_height = upng_get_height(texture);
  ;

  // Map the UV coordinate to the full texture width and height
  // Truncating within the allocated dimensions at the end of these lines is a
  // messy hack to make sure we are not trying to write to a value outside of
  // allocated memory GPU's take care of this using Fill Convention. We are
  // doing it the old fashioned way Note that this may result in some 'tears'
  // between faces
  int tex_x = abs((int)(interpolated_u * texture_width)) % texture_width;
  int tex_y = abs((int)(interpolated_v * texture_height)) % texture_height;

  // invert 1/w so pixels that are closer to cam have smaller values
  interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

  // As long as the current pixel is in front of what is there currently
  // (i.e., depth value of this pixel is LESS than the one previously stored in
  // z-buffer)...
  if (interpolated_reciprocal_w < get_zbuffer_at(x, y)) {
    // get buffer of colors from the texture
    uint32_t *texture_buffer = (uint32_t *)upng_get_buffer(texture);
    // ...draw the pixel
    draw_pixel(x, y, texture_buffer[(texture_width * tex_y) + tex_x]);
    // ... and update the z-buffer value with the 1/w (1 / old z in camera
    // space) of this current pixel
    set_zbuffer_at(x, y, interpolated_reciprocal_w);
  }
}

void draw_textured_triangle(int x0, int y0, float z0, float w0, float u0,
                            float v0, int x1, int y1, float z1, float w1,
                            float u1, float v1, int x2, int y2, float z2,
                            float w2, float u2, float v2, upng_t *texture) {
  // We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
  if (y0 > y1) {
    int_swap(&y0, &y1);
    int_swap(&x0, &x1);
    float_swap(&z0, &z1);
    float_swap(&w0, &w1);
    float_swap(&u0, &u1);
    float_swap(&v0, &v1);
  }
  if (y1 > y2) {
    int_swap(&y1, &y2);
    int_swap(&x1, &x2);
    float_swap(&z1, &z2);
    float_swap(&w1, &w2);
    float_swap(&u1, &u2);
    float_swap(&v1, &v2);
  }
  if (y0 > y1) {
    int_swap(&y0, &y1);
    int_swap(&x0, &x1);
    float_swap(&z0, &z1);
    float_swap(&w0, &w1);
    float_swap(&u0, &u1);
    float_swap(&v0, &v1);
  }

  // Flip the V component to account for inverted UV-coordinates (V grows
  // downwards)
  v0 = 1.0 - v0;
  v1 = 1.0 - v1;
  v2 = 1.0 - v2;

  // Create vector points and texture coords after we sort the vertices
  vec4_t point_a = {x0, y0, z0, w0};
  vec4_t point_b = {x1, y1, z1, w1};
  vec4_t point_c = {x2, y2, z2, w2};
  tex2_t a_uv = {u0, v0};
  tex2_t b_uv = {u1, v1};
  tex2_t c_uv = {u2, v2};

  ///////////////////////////////////////////////////////
  // Render the upper part of the triangle (flat-bottom)
  ///////////////////////////////////////////////////////
  float inv_slope_1 = 0;
  float inv_slope_2 = 0;

  if (y1 - y0 != 0)
    inv_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
  if (y2 - y0 != 0)
    inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

  if (y1 - y0 != 0) {
    for (int y = y0; y <= y1; y++) {
      int x_start = x1 + (y - y1) * inv_slope_1;
      int x_end = x0 + (y - y0) * inv_slope_2;

      if (x_end < x_start) {
        int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
      }

      for (int x = x_start; x < x_end; x++) {
        // Draw our pixel with the color that comes from the texture
        draw_texel(x, y, texture, point_a, point_b, point_c, a_uv, b_uv, c_uv);
      }
    }
  }

  ///////////////////////////////////////////////////////
  // Render the bottom part of the triangle (flat-top)
  ///////////////////////////////////////////////////////
  inv_slope_1 = 0;
  inv_slope_2 = 0;

  if (y2 - y1 != 0)
    inv_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
  if (y2 - y0 != 0)
    inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

  if (y2 - y1 != 0) {
    for (int y = y1; y <= y2; y++) {
      int x_start = x1 + (y - y1) * inv_slope_1;
      int x_end = x0 + (y - y0) * inv_slope_2;

      if (x_end < x_start) {
        int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
      }

      for (int x = x_start; x < x_end; x++) {
        // Draw our pixel with the color that comes from the texture
        draw_texel(x, y, texture, point_a, point_b, point_c, a_uv, b_uv, c_uv);
      }
    }
  }
}

