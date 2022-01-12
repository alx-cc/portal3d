#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include "matrix.h"

typedef struct {
    vec3_t position;
    vec3_t direction;
    vec3_t fwd_velocity;
    float yaw_angle;
    float pitch_angle;
} camera_t;

void init_camera(vec3_t position, vec3_t direction);
void move_camera_y(float offset);
void move_camera_x(float offset);

void set_camera_direction(vec3_t direction);
void set_camera_fwd_vel(vec3_t fwd_velocity);
void set_camera_position(vec3_t position);

void rotate_camera_z(float yaw);
void rotate_camera_x(float pitch);

vec3_t get_camera_position();
vec3_t get_camera_direction();
vec3_t get_camera_fwd_vel();

float get_camera_yaw();
float get_camera_pitch();

vec3_t get_camera_lookat_target(void);

#endif
