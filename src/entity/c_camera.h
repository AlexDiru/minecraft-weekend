#ifndef C_CAMERA_H
#define C_CAMERA_H

#include "ecs.h"
#include "../util/camera.h"

struct CameraComponent {
    vec3s offset;
    PerspectiveCamera camera;
};

#endif