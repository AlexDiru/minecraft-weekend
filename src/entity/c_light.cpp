#include "c_light.h"
#include "ecs.h"
#include "../world/world.h"
#include "../world/light.h"

static void tick(LightComponent *c_light, Entity* entity) {
    struct PositionComponent *c_position = entity->componentManager->getPositionComponent(entity);

    bool changed = ivec3scmp(c_position->block, c_light->last.pos) ||
        c_light->flags.enabled != c_light->last.enabled;

    if (changed) {
        torchlight_remove(entity->ecs->world, c_light->last.pos);

        // reset block light if it was removed
        Block block = BLOCKS[world_get_block(entity->ecs->world, c_light->last.pos)];
        if (block.can_emit_light) {
            torchlight_add(
                entity->ecs->world,
                c_light->last.pos,
                block.get_torchlight(entity->ecs->world, c_light->last.pos));
        }
    }

    if (c_light->flags.enabled && changed) {
        torchlight_add(entity->ecs->world, c_position->block, c_light->light);

        c_light->last.pos = c_position->block;
        c_light->last.light = c_light->light;
    }

    c_light->last.enabled = c_light->flags.enabled;
}

void c_light_init(ECS *ecs) {
    ECSSystem system;
    system.init = NULL;
    system.destroy = NULL;
    system.render = NULL;
    system.update = NULL;
    system.tick = (ECSSubscriber) tick;

    ecs_register(C_LIGHT, LightComponent, ecs, system);
}