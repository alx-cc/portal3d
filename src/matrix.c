#include "matrix.h"
#include <math.h>

mat4_t mat4_identity(void) {
    mat4_t m = {{
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    }};
    return m;
}

mat4_t mat4_make_scale(float sx, float sy, float sz) {
    mat4_t matrix = mat4_identity();
    matrix.m[0][0] = sx;
    matrix.m[1][1] = sy;
    matrix.m[2][2] = sz;
    return matrix;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v) {
    vec4_t result;
    result.x = (m.m[0][0] * v.x) + (m.m[0][1] * v.y) + (m.m[0][2] * v.z) + (m.m[0][3] * v.w);
    result.y = (m.m[1][0] * v.x) + (m.m[1][1] * v.y) + (m.m[1][2] * v.z) + (m.m[1][3] * v.w);
    result.z = (m.m[2][0] * v.x) + (m.m[2][1] * v.y) + (m.m[2][2] * v.z) + (m.m[2][3] * v.w);
    result.w = (m.m[3][0] * v.x) + (m.m[3][1] * v.y) + (m.m[3][2] * v.z) + (m.m[3][3] * v.w);

    return result;
}


mat4_t mat4_make_translation(float tx, float ty, float tz){

    mat4_t matrix = mat4_identity();
    matrix.m[0][3] = tx;
    matrix.m[1][3] = ty;
    matrix.m[2][3] = tz;

    return matrix;
}

mat4_t mat4_make_rotation_z(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    // We want to return a matrix of the following form:
        // cos(a) -sin(a)  0   0
        // sin(a)  cos(a)  0   0
        // 0       0       1   0
        // 0       0       0   1
    mat4_t matrix = mat4_identity();
    matrix.m[0][0] = c;
    matrix.m[0][1] = -s;
    matrix.m[1][0] = s;
    matrix.m[1][1] = c;
    return matrix;
}

mat4_t mat4_make_rotation_x(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    // We want to return a matrix of the following form:
        // 1  0       0       0
        // 0  cos(a) -sin(a)  0
        // 0  sin(a)  cos(a)  0
        // 0  0       0       1
    mat4_t matrix = mat4_identity();
    matrix.m[1][1] = c;
    matrix.m[1][2] = -s;
    matrix.m[2][1] = s;
    matrix.m[2][2] = c;
    return matrix;
}

mat4_t mat4_make_rotation_y(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    // We want to return a matrix of the following form:
        // cos(a)   0  sin(a)  0
        // 0        1  0       0
        // -sin(a)  0  cos(a)  0
        // 0        0  0       1
    // Note that with y rotation, we're swapping the pos and neg sin(a)'s to maintain CW direction (to match z and x rotation)
    mat4_t matrix = mat4_identity();
    matrix.m[0][0] = c;
    matrix.m[0][2] = s;
    matrix.m[2][0] = -s;
    matrix.m[2][2] = c;
    return matrix;
}

mat4_t mat4_mul_mat4(mat4_t a, mat4_t b) {
    mat4_t result;
    for (int i = 0; i < 4; i++) {
       for (int j = 0; j < 4; j++) {
           result.m[i][j] = (a.m[i][0] * b.m[0][j]) + (a.m[i][1] * b.m[1][j]) + (a.m[i][2] * b.m[2][j]) + (a.m[i][3] * b.m[3][j]);
        }
    }
    return result;
}

mat4_t mat4_make_perspective(float fov, float aspect_ratio, float znear, float zfar) {
    // Each scalar expression multiplied in the way we do below normalizes each componenent as follows:
    // scaling x by aspect ratio takes aspect ratio into account, scaling x and y by the 1/tan formula takes fov into
    // account, and scaling z by the zfar/znear formula takes the distance between starting view and edge of horizon into
    // account.
    mat4_t result = {{{ 0 }}}; // initializes all components to 0
    result.m[0][0] = aspect_ratio * (1 / tan(fov/2)); // aspect_ratio multiplicaton takes aspect ratio into account
    result.m[1][1] = 1 / tan(fov / 2);
    result.m[2][2] = zfar / (zfar - znear);
    result.m[2][3] = (-zfar * znear) / (zfar - znear);
    result.m[3][2] = 1.0; // this operation effectively stores the original z in the 4th component to use later for persp divide
    return result;
}

vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v) {
    // multiply the projection matrix by the original vector
    vec4_t result = mat4_mul_vec4(mat_proj, v);
    // perform perspective divide by using the original z (the z before perspective matrix was applied.
    // this z was stored in 4th component (w))
    if (result.w != 0.0) {
        result.x /= result.w;
        result.y /= result.w;
        result.z /= result.w;
    }

    return result;
}


mat4_t mat4_look_at(vec3_t eye, vec3_t target, vec3_t up) {
    // subtraction of target minus the eye = forward vector
    vec3_t z = vec3_sub(target, eye);
    vec3_normalize(&z); // forward (z) vector
    // cross product of forward and up gives you right
    vec3_t x = vec3_cross(up, z); // right (x) vector
    vec3_normalize(&x);
    // cross product of forward and right gives you up. no need to normalize since the others already are
    vec3_t y = vec3_cross(z, x);

    // : x.x    x.y     x.z     -dot(x, eye) :
    // : y.x    y.y     y.z     -dot(y, eye) :
    // : z.x    z.y     z.z     -dot(z, eye) :
    // :   0      0       0                1 :
    mat4_t view_matrix = {{
        { x.x, x.y, x.z, -vec3_dot(x, eye) },
        { y.x, y.y, y.z, -vec3_dot(y, eye) },
        { z.x, z.y, z.z, -vec3_dot(z, eye) },
        {   0,   0,   0,                 1 },
    }};

    return view_matrix;
}

