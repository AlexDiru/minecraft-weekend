#include "block.h"

static ivec2s get_texture_location(World *world, ivec3s pos, enum Direction d) {
    return (ivec2s) {{ 6, 1 }};
}

void planks_init() {
    Block planks = BLOCK_DEFAULT;
    planks.id = PLANKS;
    planks.get_texture_location = get_texture_location;
    BLOCKS[PLANKS] = planks;
}