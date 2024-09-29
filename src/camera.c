#include "camera.h"

static camera_t camera = {};

// Initialize camera
void init_camera(vec3_t position, vec3_t direction) {
  camera.position = position;
  camera.direction = direction;
  camera.fwd_velocity = vec3_new(0, 0, 0);
  ;
  camera.yaw_angle = 0.0;
  camera.pitch_angle = 0.0;
}

void move_camera_y(float offset) { camera.position.y += offset; }

void move_camera_x(float offset) { camera.position.x += offset; }

void rotate_camera_z(float yaw) { camera.yaw_angle += yaw; }

void rotate_camera_x(float pitch) { camera.pitch_angle += pitch; }

void set_camera_position(vec3_t position) { camera.position = position; }

void set_camera_direction(vec3_t direction) { camera.direction = direction; }

void set_camera_fwd_vel(vec3_t fwd_velocity) {
  camera.fwd_velocity = fwd_velocity;
}

vec3_t get_camera_position() { return camera.position; }

vec3_t get_camera_direction() { return camera.direction; }

vec3_t get_camera_fwd_vel() { return camera.fwd_velocity; }

float get_camera_yaw() { return camera.yaw_angle; }

float get_camera_pitch() { return camera.pitch_angle; }

vec3_t get_camera_lookat_target(void) {
  // Initialize the target looking at the positive z-axis
  vec3_t target = {0, 0, 1};

  mat4_t camera_yaw_rotation = mat4_make_rotation_y(camera.yaw_angle);
  mat4_t camera_pitch_rotation = mat4_make_rotation_x(camera.pitch_angle);

  // Create camera rotation matrix based on yaw and pitch
  mat4_t camera_rotation = mat4_identity();
  camera_rotation = mat4_mul_mat4(camera_pitch_rotation, camera_rotation);
  camera_rotation = mat4_mul_mat4(camera_yaw_rotation, camera_rotation);

  // Update camera direction based on the rotation
  vec4_t camera_direction =
      mat4_mul_vec4(camera_rotation, vec4_from_vec3(target));
  camera.direction = vec3_from_vec4(camera_direction);

  // Offset the camera position in the direction where the camera is pointing at
  target = vec3_add(camera.position, camera.direction);

  return target;
}
