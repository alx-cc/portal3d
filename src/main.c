#include "array.h"
#include "camera.h"
#include "clipping.h"
#include "display.h"
#include "light.h"
#include "matrix.h"
#include "mesh.h"
#include "texture.h"
#include "triangle.h"
#include "upng.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

bool is_running = false;
bool is_moving_forward = false;
bool is_moving_backward = false;
bool is_rotating_left = false;
bool is_rotating_right = false;
bool is_facing_up = false;
bool is_facing_down = false;
bool is_floating_up = false;
bool is_floating_down = false;

int previous_frame_time = 0;	
float delta_time = 0;
int grid_bg;
int grid_fg;

// an array of triangles to be rendered frame by frame
// switched to static array so we don't have to reallocate a dynamic array every
// frame
#define MAX_TRIANGLES 10000
triangle_t triangles_to_render[MAX_TRIANGLES];
int num_triangles_to_render = 0;

mat4_t proj_matrix;
mat4_t view_matrix;

/**
 * Allocate required memory for color buffer and create the SDL texture
 * that is used to display it
 */
void setup(void) {
  // initialize render mode
  set_render_method(RENDER_TEXTURED);
  set_cull_method(CULL_BACKFACE);

  // initialize the scene light direction
  init_light(vec3_new(0, 0, 1));

  // initialize perspective projection matrix
  float aspect_ratio_x = (float)get_window_width() / (float)get_window_height();
  float aspect_ratio_y = (float)get_window_height() / (float)get_window_width();
  float fov_y = 3.14159 / 3.0; // 60 deg in radians
  float fov_x = atan(tan(fov_y / 2) * aspect_ratio_x) * 2.0;
  float z_near = 0.1;
  float z_far = 100.0;
  proj_matrix = mat4_make_perspective(fov_y, aspect_ratio_y, z_near, z_far);

  // Initialize frustum planes with a point and a normal
  init_frustum_planes(fov_x, fov_y, z_near, z_far);

  // Load mesh data
  load_mesh("./assets/f22.obj", "./assets/f22.png", vec3_new(1, 1, 1),
            vec3_new(-3, 0, +8), vec3_new(0, 0, 0));
  load_mesh("./assets/efa.obj", "./assets/efa.png", vec3_new(1, 1, 1),
            vec3_new(+3, 0, +9), vec3_new(0, 0, 0));
}

/**
 * Read events from keyboard
 */
void process_input(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
		    
            // Movement controls
            case SDLK_w: is_moving_forward = true; break;
            case SDLK_s: is_moving_backward = true; break;
            case SDLK_a: is_rotating_left = true; break;
            case SDLK_d: is_rotating_right = true; break;
	    case SDLK_UP: is_facing_up = true; break; 
	    case SDLK_DOWN: is_facing_down = true; break; 
	    case SDLK_SPACE: is_floating_up = true; break;
	    case SDLK_LSHIFT: is_floating_down = true; break;

            // Render methods
            case SDLK_1: set_render_method(RENDER_WIRE); break;
            case SDLK_2: set_render_method(RENDER_WIRE_VERTEX); break;
            case SDLK_3: set_render_method(RENDER_FILL_TRIANGLE); break;
            case SDLK_4: set_render_method(RENDER_FILL_TRIANGLE_WIRE); break;
            case SDLK_5: set_render_method(RENDER_TEXTURED); break;
            case SDLK_6: set_render_method(RENDER_TEXTURED_WIRE); break;

            // Backface culling toggles
            case SDLK_7: set_cull_method(CULL_BACKFACE); break;
            case SDLK_8: set_cull_method(CULL_NONE); break;

            // Quit
            case SDLK_ESCAPE:
                is_running = false;
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            // Stop movement
            case SDLK_w: is_moving_forward = false; break;
            case SDLK_s: is_moving_backward = false; break;
            case SDLK_a: is_rotating_left = false; break;
            case SDLK_d: is_rotating_right = false; break;
	    case SDLK_UP: is_facing_up = false; break; 
	    case SDLK_DOWN: is_facing_down = false; break;
	    case SDLK_SPACE: is_floating_up = false; break;
	    case SDLK_LSHIFT: is_floating_down = false; break;

            }
            break;
        }
    }
}


void update_camera(float delta_time) {
    if (is_moving_forward) {
        vec3_t forward = vec3_mul(get_camera_direction(), 5.0 * delta_time);
        set_camera_position(vec3_add(get_camera_position(), forward));
    }
    if (is_moving_backward) {
        vec3_t backward = vec3_mul(get_camera_direction(), 5.0 * delta_time);
        set_camera_position(vec3_sub(get_camera_position(), backward));
    }
    if (is_rotating_left) {
        rotate_camera_z(-1.0 * delta_time);
    }
    if (is_rotating_right) {
        rotate_camera_z(1.0 * delta_time);
    }
    if (is_facing_up) {
        rotate_camera_x(1.0 * delta_time);
    }
    if (is_facing_down) {
        rotate_camera_x(-1.0 * delta_time);
    }
    if (is_floating_up) {
        move_camera_y(3.0 * delta_time);
    }
    if (is_floating_down) {
        move_camera_y(-3.0 * delta_time);
    }
}



void update(void) {
  // block program until we have reached the millisecond duration we designated
  // for 1 frame in FRAME_TARGET_TIME (for 30 fps that's 33.333ms) this locks
  // our animation to our constant framerate so that fps is machine independent:
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
  // only delay execution if we are running too fast:
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
    SDL_Delay(time_to_wait);

  delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0;

// Update the camera position and orientation based on input
    update_camera(delta_time);

  // calculate how many ms have passed since last frame
  previous_frame_time =
      SDL_GetTicks(); // how many ms have passed since SDL_init()


  // Initialize counter of triangles to render for the current frame
  num_triangles_to_render = 0;

  // Loop all the meshes of our scene
  for (int mesh_index = 0; mesh_index < get_num_meshes(); mesh_index++) {
    mesh_t *mesh = get_mesh(mesh_index);

    // Create scale, translation and rotation matrices that will be used to
    // multiply the mesh vertices, passing in the corresponding values (that are
    // changing over time) in the mesh struct of the corresponding object
    mat4_t scale_matrix =
        mat4_make_scale(mesh->scale.x, mesh->scale.y, mesh->scale.z);
    mat4_t translation_matrix = mat4_make_translation(
        mesh->translation.x, mesh->translation.y, mesh->translation.z);
    mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh->rotation.x);
    mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh->rotation.y);
    mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh->rotation.z);

    // Update camera look at target to create view matrix
    vec3_t target = get_camera_lookat_target();
    vec3_t up_direction = vec3_new(0, 1, 0);

    // Create the view matrix
    view_matrix = mat4_look_at(get_camera_position(), target, up_direction);

    // loop all triangle faces of our mesh
    int num_faces = array_length(mesh->faces);
    for (int i = 0; i < num_faces; i++) {
      face_t mesh_face = mesh->faces[i];

      vec3_t face_vertices[3];
      face_vertices[0] = mesh->vertices[mesh_face.a - 1];
      face_vertices[1] = mesh->vertices[mesh_face.b - 1];
      face_vertices[2] = mesh->vertices[mesh_face.c - 1];

      vec4_t transformed_vertices[3];

      // loop all 3 vertices of this current face and apply transformations
      for (int j = 0; j < 3; j++) {
        vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

        // Create a World Matrix combining scale, rotation and translation
        // matrices Since matrix multiplication is not commutative, order
        // matters! (scale, rotate, translate)
        mat4_t world_matrix = mat4_identity();
        // multiply w_m by scale to store scale scalars within it
        world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
        // multiply w_m by rotation matrices to store rotation scalars within it
        world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
        world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
        world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
        // multiply w_m by translation matrix to store translation scalars
        // within it
        world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

        // Multiply world matrix by the original vector to transform scene to
        // world space
        transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

        // Multiply the view matrix by the vector to then transform scene to
        // camera space
        transformed_vertex = mat4_mul_vec4(view_matrix, transformed_vertex);

        // Save this transformed vertex (after being scaled/translated/rotated)
        // in the array of transformed vertices
        transformed_vertices[j] = transformed_vertex;
      }

      // label each vertex of this given triangle for the sake of simplicity
      vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]);
      vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]);
      vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]);

      // culling step 1: find vectors B-A and C-A
      vec3_t vector_ab = vec3_sub(vector_b, vector_a);
      vec3_t vector_ac = vec3_sub(vector_c, vector_a);
      vec3_normalize(&vector_ab);
      vec3_normalize(&vector_ac);

      // culling step 2: take their cross product and find the perpendicular
      // normal
      vec3_t normal = vec3_cross(vector_ab, vector_ac);
      vec3_normalize(&normal);

      // culling step 3: find the camera ray vector by subtracting camera
      // position from point A
      vec3_t origin = {0, 0, 0};
      vec3_t camera_ray = vec3_sub(origin, vector_a);

      // culling step 4: take dot product between normal and camera ray,
      // if the dot product is < 0, face is pointing away from camera, do not
      // display the face
      float dot_normal_camera = vec3_dot(normal, camera_ray);

      // Backface culling (if enabled by user)
      if (is_cull_backface()) {

        // if the face normal is pointing away from camera ray...
        if (dot_normal_camera < 0) {
          //...bypass the following section that would normally project this
          //face
          continue;
        }
      }

      //////////////////
      // CLIPPING LOGIC:
      //////////////////

      // Create a polygon from the original transformed triangle to be clipped
      polygon_t polygon = create_polygon_from_triangle(
          vec3_from_vec4(transformed_vertices[0]),
          vec3_from_vec4(transformed_vertices[1]),
          vec3_from_vec4(transformed_vertices[2]), mesh_face.a_uv,
          mesh_face.b_uv, mesh_face.c_uv);

      // Clip the polygon and returns a new polygon with potential new vertices
      clip_polygon(&polygon);

      // Break the clipped polygon apart back into individual triangles
      triangle_t triangles_after_clipping[MAX_POLY_TRIANGLES];
      int num_triangles_after_clipping = 0;

      triangles_from_polygon(&polygon, triangles_after_clipping,
                             &num_triangles_after_clipping);

      // Loop all assembled triangles after clipping
      for (int t = 0; t < num_triangles_after_clipping; t++) {
        triangle_t triangle_after_clipping = triangles_after_clipping[t];

        vec4_t projected_points[3];

        // loop all vertices of triangles NOT excluded by backface culling and
        // finally project them
        for (int j = 0; j < 3; j++) {

          // project the current vertex (multiply it by the projection matrix)
          projected_points[j] =
              mat4_mul_vec4(proj_matrix, triangle_after_clipping.points[j]);

          // Perform perspective divide
          if (projected_points[j].w != 0) {
            projected_points[j].x /= projected_points[j].w;
            projected_points[j].y /= projected_points[j].w;
            projected_points[j].z /= projected_points[j].w;
          }

          // On-screen y coordinates are processed in the opposite direction in
          // which they are read in from .obj files, so we will invert y
          // coordinates here
          projected_points[j].y *= -1;

          // scale into view using window dimensions
          projected_points[j].x *= (get_window_width() / 2.0);
          projected_points[j].y *= (get_window_height() / 2.0);

          // scale and translate the projected points to the middle of screen
          projected_points[j].x += (get_window_width() / 2.0);
          projected_points[j].y += (get_window_height() / 2.0);
        }

        // Calculate the average depth of each face based on their respective
        // vertices after transformation

        // Calculate shade intensity based on how aligned the face normal and
        // light normal are
        float light_intensity_factor = -vec3_dot(normal, get_light_direction());

        // Calculate triangle color based on light angle
        uint32_t triangle_color =
            light_apply_intensity(mesh_face.color, light_intensity_factor);

        // Now using the data we created, we actually create the triangle to
        // project
        triangle_t triangle_to_render = {
            // assign triangle points (taken from the points we just processed
            // (projected))
            .points = {{projected_points[0].x, projected_points[0].y,
                        projected_points[0].z, projected_points[0].w},
                       {projected_points[1].x, projected_points[1].y,
                        projected_points[1].z, projected_points[1].w},
                       {projected_points[2].x, projected_points[2].y,
                        projected_points[2].z, projected_points[2].w}},

            // assign triangle UV texture coordinates (taken from this object's
            // mesh's face struct)
            .texcoords = {{triangle_after_clipping.texcoords[0].u,
                           triangle_after_clipping.texcoords[0].v},
                          {triangle_after_clipping.texcoords[1].u,
                           triangle_after_clipping.texcoords[1].v},
                          {triangle_after_clipping.texcoords[2].u,
                           triangle_after_clipping.texcoords[2].v}},
            // assign this triangle's color
            .color = triangle_color,
            .texture = mesh->texture};

        // save the projected triangles in the array of triangles to render
        if (num_triangles_to_render < MAX_TRIANGLES) {
          triangles_to_render[num_triangles_to_render++] = triangle_to_render;
        }
      }
    }
  }
}

// TODO : Something in this fct is causing slower performance and choppy-looking
// edges 
void render(void) {

  // Clear all arrays to get ready for next frame
  clear_color_buffer(0xFF000000);
  clear_z_buffer();

  draw_grid(0x00040404, 0x00020000);
  // draw_horizon();

  // loop all projected points and render them
  for (int i = 0; i < num_triangles_to_render; i++) {
    triangle_t triangle = triangles_to_render[i];

    // if render mode is set to either fill or fill+wireframe...
    if (should_render_filled_triangles()) {
      // draw filled triangle
      draw_filled_triangle(
          triangle.points[0].x, triangle.points[0].y, triangle.points[0].z,
          triangle.points[0].w, // vertex A
          triangle.points[1].x, triangle.points[1].y, triangle.points[1].z,
          triangle.points[1].w, // vertex B
          triangle.points[2].x, triangle.points[2].y, triangle.points[2].z,
          triangle.points[2].w, // vertex C
          triangle.color);
    }

    // if render mode is set to either wireframe, wireframe+vertices
    // fill+wireframe or textured+fireframe...
    if (should_render_wireframe()) {
      // draw unfilled triangle
      draw_triangle(triangle.points[0].x, triangle.points[0].y, // vertex A
                    triangle.points[1].x, triangle.points[1].y, // vertex B
                    triangle.points[2].x, triangle.points[2].y, // vertex C
                    0xFF999999);
    }

    // if render mode is set to texture or texture+wireframe...
    if (should_render_textured_triangles()) {
      // draw textured triangle
      draw_textured_triangle(
          triangle.points[0].x, triangle.points[0].y, triangle.points[0].z,
          triangle.points[0].w, triangle.texcoords[0].u,
          triangle.texcoords[0].v, // vertex A
          triangle.points[1].x, triangle.points[1].y, triangle.points[1].z,
          triangle.points[1].w, triangle.texcoords[1].u,
          triangle.texcoords[1].v, // vertex B
          triangle.points[2].x, triangle.points[2].y, triangle.points[2].z,
          triangle.points[2].w, triangle.texcoords[2].u,
          triangle.texcoords[2].v, // vertex C
          triangle.texture);
    }

    // if render mode is set to wireframe+vertices, render little rectangles at
    // each vertex
    if (should_render_wire_vertex()) {
      draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6,
                0xFFFF0000);
      draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6,
                0xFFFF0000);
      draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6,
                0xFFFF0000);
    }
  }

  // Finally draw the color buffer to the SDL window and actually present the
  // color buffer
  render_color_buffer();
}

// free the memory that was dynamically allocated by program
void free_resources(void) {
  free_meshes();
  destroy_window();
}

int main(void) {
  // use boolean flag from initialize_window() to set is_running flag
  is_running = initialize_window();

  // allocate memory for and create required structures
  setup();

  // our game loop
  while (is_running) {
    process_input();
    update();
    render();
  }

  destroy_window();
  free_resources();

  return 0;
}
