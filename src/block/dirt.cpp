#include "block.h"

static ivec2s get_texture_location(World *world, ivec3s pos, enum Direction d) {
    return (ivec2s) {{ 2, 0 }};
}

void dirt_init() {
    Block dirt = BLOCK_DEFAULT;
    dirt.id = DIRT;
    dirt.get_texture_location = get_texture_location;
    BLOCKS[DIRT] = dirt;
}