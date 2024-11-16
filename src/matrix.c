#include "matrix.h"
#include <math.h>

// Return a 4x4 identity matrix
// Identity matrices are used as the starting point for transformations,
// with no effect on vectors when multiplied.
mat4_t mat4_identity(void) {
  // We want to return a matrix of the following form:
  // 1   0   0   0
  // 0   1   0   0
  // 0   0   1   0
  // 0   0   0   1
  mat4_t m = {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
  return m;
}

// Create a 4x4 scaling matrix
// The diagonal elements (sx, sy, sz) scale the x, y, and z coordinates
// respectively, while leaving translation and perspective components unchanged.
mat4_t mat4_make_scale(float sx, float sy, float sz) {
  // We want to return a matrix of the following form:
  // sx  0   0   0
  // 0   sy  0   0
  // 0   0   sz  0
  // 0   0   0   1
  mat4_t matrix = mat4_identity();
  matrix.m[0][0] = sx;
  matrix.m[1][1] = sy;
  matrix.m[2][2] = sz;
  return matrix;
}

// Multiply a 4x4 matrix with a 4D vector
// This applies transformations such as scaling, rotation, translation, or
// projection stored in the matrix to the vector.
vec4_t mat4_mul_vec4(mat4_t m, vec4_t v) {
  vec4_t result;
  result.x = (m.m[0][0] * v.x) + (m.m[0][1] * v.y) + (m.m[0][2] * v.z) +
             (m.m[0][3] * v.w);
  result.y = (m.m[1][0] * v.x) + (m.m[1][1] * v.y) + (m.m[1][2] * v.z) +
             (m.m[1][3] * v.w);
  result.z = (m.m[2][0] * v.x) + (m.m[2][1] * v.y) + (m.m[2][2] * v.z) +
             (m.m[2][3] * v.w);
  result.w = (m.m[3][0] * v.x) + (m.m[3][1] * v.y) + (m.m[3][2] * v.z) +
             (m.m[3][3] * v.w);

  return result;
}

// Create a 4x4 translation matrix
// Adds a translation offset (tx, ty, tz) to a vector by modifying the last
// column of the matrix.
mat4_t mat4_make_translation(float tx, float ty, float tz) {

  mat4_t matrix = mat4_identity();
  matrix.m[0][3] = tx;
  matrix.m[1][3] = ty;
  matrix.m[2][3] = tz;

  return matrix;
}

////////////////////////////////////////////////////////////////////////
// ROTATION MATRIX FUNCTIONS
//
// Start with an identity matrix, then set the matrix elements such that
// when multiplied with another matrix, they apply a rotatation about a 
// specific axis at the desired angle by applying the rotation to the 2 
// axes other than the desired one. 
// (e.g., if you want to rotate about z, you would rotate x and rotate y). 
// The remaining elements of the matrix retain the values of an identity 
// matrix so that they apply no change.
//
// Recall that, in 2D space, if a vector is rotated about the origin by 
// some angle a, you can find the coordinates of the new rotated vector 
// by multiplying:
// x * cos(a) - y * sin(a) for the new x, and
// x * sin(a) + y * cos(a) for the new y.
//
// The placement of the trig functions in these matrices result in the
// above formula being applied to the appropriate coordinates on the
// appropriate plane. e.g., applying that formula to x and z result in
// a transformation matrix that results in a rotation about the y axis.
/////////////////////////////////////////////////////////////////////////

// Rotate around Z axis by affecting the XY plane
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

// Rotate around X axis by affecting the YZ plane
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

// Rotate around Y axis by affecting the XZ plane, swapping pos and neg
// sin(a)'s to maintain CW direction (in order to match z and x rotation)
mat4_t mat4_make_rotation_y(float angle) {
  float c = cos(angle);
  float s = sin(angle);
  // We want to return a matrix of the following form:
  // cos(a)   0  sin(a)  0
  // 0        1  0       0
  // -sin(a)  0  cos(a)  0
  // 0        0  0       1
  // (swapping the pos and neg sin(a)'s to maintain CW direction)
  mat4_t matrix = mat4_identity();
  matrix.m[0][0] = c;
  matrix.m[0][2] = s;
  matrix.m[2][0] = -s;
  matrix.m[2][2] = c;
  return matrix;
}

// END OF ROTATION MATRIX FUNCTIONS /////////////////////////////////////

// Multiply two 4x4 matrices
// Combines transformations (e.g., scaling, rotation, translation, projection)
// into a single matrix by performing dot products between rows of `a`
// and columns of `b`.
mat4_t mat4_mul_mat4(mat4_t a, mat4_t b) {
  mat4_t result;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      result.m[i][j] = (a.m[i][0] * b.m[0][j]) + (a.m[i][1] * b.m[1][j]) +
                       (a.m[i][2] * b.m[2][j]) + (a.m[i][3] * b.m[3][j]);
    }
  }
  return result;
}

// Create a perspective projection matrix
// This matrix maps 3D points to a 2D plane while preserving depth.
// - FOV controls the field of view angle.
// - Aspect ratio scales the x-coordinate to fit the screen dimensions.
// - znear and zfar define the near and far clipping planes.
// - The w-component stores depth for perspective division.
mat4_t mat4_make_perspective(float fov, float aspect_ratio, float znear,
                             float zfar) {
  // Each scalar expression multiplied in the way we do below normalizes each
  // componenent as follows: scaling x by aspect ratio takes aspect ratio into
  // account, scaling x and y by the 1/tan formula takes fov into account, and
  // scaling z by the zfar/znear formula takes the distance between starting
  // view and edge of horizon into account.
  mat4_t result = {{{0}}}; // initializes all components to 0
  result.m[0][0] =
      aspect_ratio * (1/tan(fov/2)); // aspect_ratio multiplicaton takes aspect ratio into account
  result.m[1][1] = 1 / tan(fov / 2);
  result.m[2][2] = zfar / (zfar - znear);
  result.m[2][3] = (-zfar * znear) / (zfar - znear);
  result.m[3][2] = 1.0; // this operation effectively stores the original z in
                        // the 4th component to use later for persp divide
  return result;
}

// Multiply a vector by a projection matrix and apply perspective divide
// - First, multiply the vector by the projection matrix to apply depth scaling.
// - Then, divide the x, y, and z coordinates by the w-component to "normalize" 
//   the vector, mapping it to normalized device coordinates (NDC), where
//   x, y, and z range from -1 to 1 for points visible on screen.
vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v) {
  // multiply the projection matrix by the original vector
  vec4_t result = mat4_mul_vec4(mat_proj, v);
  // perform perspective divide by using the original z (the z before
  // perspective matrix was applied. this z was stored in 4th component (w))
  if (result.w != 0.0) {
    result.x /= result.w;
    result.y /= result.w;
    result.z /= result.w;
  }

  return result;
}

// Create a view matrix for a camera looking at a target from a given position
// - This matrix transforms world coordinates into the camera's local view space.
// - The forward vector (z) is calculated as the direction from the camera 
//   position (eye) to the target. This defines what the camera is looking at.
// - The right vector (x) is the cross product of the up vector and the forward 
//   vector. This ensures the camera's x-axis is perpendicular to both its 
//   "look direction" and its up direction.
// - The up vector (y) is recalculated as the cross product of the forward and 
//   right vectors. This ensures all three vectors (x, y, z) form an orthogonal 
//   basis for the camera's coordinate system.
// - The negative dot products in the last column adjust the origin of the 
//   view space to the camera's position, ensuring the camera itself becomes 
//   the "center" of the view.
mat4_t mat4_look_at(vec3_t eye, vec3_t target, vec3_t up) {
  // subtraction of target minus the eye = forward vector
  vec3_t z = vec3_sub(target, eye);
  vec3_normalize(&z); // forward (z) vector
  // cross product of forward and up gives you right
  vec3_t x = vec3_cross(up, z); // right (x) vector
  vec3_normalize(&x);
  // cross product of forward and right gives you up. no need to normalize since
  // the others already are
  vec3_t y = vec3_cross(z, x);

  // : x.x    x.y     x.z     -dot(x, eye) :
  // : y.x    y.y     y.z     -dot(y, eye) :
  // : z.x    z.y     z.z     -dot(z, eye) :
  // :   0      0       0                1 :
  mat4_t view_matrix = {{
      {x.x, x.y, x.z, -vec3_dot(x, eye)},
      {y.x, y.y, y.z, -vec3_dot(y, eye)},
      {z.x, z.y, z.z, -vec3_dot(z, eye)},
      {0, 0, 0, 1},
  }};

  return view_matrix;
}
