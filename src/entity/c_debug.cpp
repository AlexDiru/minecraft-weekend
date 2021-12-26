#include "c_debug.h"
#include "ecs.h"

static void render(DebugComponent *c_debug, Entity entity) {

}

void c_debug_init(ECS *ecs) {
    ECSSystem system;
    system.init = NULL;
    system.destroy = NULL;
    system.render = (ECSSubscriber) render;
    system.update = NULL;
    system.tick = NULL;

    ecs_register(C_DEBUG, DebugComponent, ecs, system);
}