#ifndef CAMERA_H
#define CAMERA_H

#include "../util/util.h"

#define CAMERA_TYPE_LAST CAMERA_PERSPECTIVE
enum CameraType {
    CAMERA_ORTHO, CAMERA_PERSPECTIVE
};

struct ViewProj {
    mat4s view, proj;
};

struct OrthoCamera {
    ViewProj view_proj;
    vec2s position, min, max;
};

struct PerspectiveCamera {
    struct ViewProj view_proj;
    vec3s position, direction, up, right;
    f32 pitch, yaw, fov, aspect, znear, zfar;
};

void perspective_camera_init(PerspectiveCamera *self, f32 fov);
void perspective_camera_update(PerspectiveCamera *self);

void ortho_camera_init(OrthoCamera *self, vec2s min, vec2s max);
void ortho_camera_update(OrthoCamera *self);

#endif