#include "block.h"

static ivec2s get_texture_location(World *world, ivec3s pos, enum Direction d) {
    return (ivec2s) {{ 5, 1 }};
}

void clay_init() {
    Block clay = BLOCK_DEFAULT;
    clay.id = CLAY;
    clay.get_texture_location = get_texture_location;
    BLOCKS[CLAY] = clay;
}