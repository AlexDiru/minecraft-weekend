#include "c_position.h"
#include "ecs.h"
#include "../world/world.h"

static void tick(PositionComponent *c_position, Entity entity) {
    ivec3s block = world_pos_to_block(c_position->position);
    ivec3s offset = world_pos_to_offset(block);

    if (ivec3scmp(block, c_position->block)) {
        c_position->block = block;
        c_position->block_changed = true;
    } else {
        c_position->block_changed = false;
    }

    if (ivec3scmp(offset, c_position->offset)) {
        c_position->offset = offset;
        c_position->offset_changed = true;
    } else {
        c_position->offset_changed = false;
    }
}

void c_position_init(ECS *ecs) {

    ECSSystem system;
    system.init = NULL;
    system.destroy = NULL;
    system.render = NULL;
    system.update = NULL;
    system.tick = (ECSSubscriber) tick;

    ecs_register(C_POSITION, struct PositionComponent, ecs, system);
}