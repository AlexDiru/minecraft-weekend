#include "ecs.h"
#include "c_camera.h"
#include "c_position.h"

static void init(CameraComponent *c_camera, Entity* entity) {
    struct PositionComponent *c_position = entity->componentManager->getPositionComponent(entity);
    perspective_camera_init(&c_camera->camera, radians(75.0f));
    c_camera->camera.position = glms_vec3_add(c_position->position, c_camera->offset);
}

static void update(CameraComponent *c_camera, Entity* entity) {
    struct PositionComponent *c_position = entity->componentManager->getPositionComponent(entity);
    c_camera->camera.position = glms_vec3_add(c_position->position, c_camera->offset);
    perspective_camera_update(&c_camera->camera);
}

void c_camera_init(ECS *ecs) {
    ECSSystem system;
    system.init = (ECSSubscriber) init;
    system.destroy = NULL;
    system.render = NULL;
    system.update = (ECSSubscriber) update;
    system.tick = NULL;

    ecs_register(C_CAMERA, CameraComponent, ecs, system);
}