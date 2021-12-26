#include "ecs.h"
#include "c_camera.h"
#include "c_position.h"

static void init(CameraComponent *c_camera, Entity entity) {
    struct PositionComponent *c_position = (PositionComponent*)ecs_get(entity, C_POSITION);
    perspective_camera_init(&c_camera->camera, radians(75.0f));
    c_camera->camera.position = glms_vec3_add(c_position->position, c_camera->offset);
}

static void update(CameraComponent *c_camera, Entity entity) {
    struct PositionComponent *c_position = (PositionComponent*)ecs_get(entity, C_POSITION);
    c_camera->camera.position = glms_vec3_add(c_position->position, c_camera->offset);
    perspective_camera_update(&c_camera->camera);
}

void c_camera_init(ECS *ecs) {
    ecs_register(C_CAMERA, CameraComponent, ecs, ((union ECSSystem) {
        .init = (ECSSubscriber) init,
        .destroy = NULL,
        .render = NULL,
        .update = (ECSSubscriber) update,
        .tick = NULL
    }));
}