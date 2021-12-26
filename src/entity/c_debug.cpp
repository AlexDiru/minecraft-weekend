#include "c_debug.h"
#include "ecs.h"

static void render(DebugComponent *c_debug, Entity entity) {

}

void c_debug_init(ECS *ecs) {
    ecs_register(C_DEBUG, DebugComponent, ecs, ((union ECSSystem) {
        .init = NULL,
        .destroy = NULL,
        .render = (ECSSubscriber) render,
        .update = NULL,
        .tick = NULL
    }));
}