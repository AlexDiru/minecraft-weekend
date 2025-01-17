#include "block.h"

static ivec2s get_texture_location(World *world, ivec3s pos, enum Direction d) {
    return (ivec2s) {{ 4, 1 }};
}

void leaves_init() {
    Block leaves = BLOCK_DEFAULT;
    leaves.id = LEAVES;
    leaves.transparent = true;
    leaves.get_texture_location = get_texture_location;
    BLOCKS[LEAVES] = leaves;
}