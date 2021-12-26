#include "block.h"

static ivec2s get_texture_location(World *world, ivec3s pos, enum Direction d) {
    return (ivec2s) {{ 0, 1 }};
}

void sand_init() {
    Block sand = BLOCK_DEFAULT;
    sand.id = SAND;
    sand.get_texture_location = get_texture_location;
    BLOCKS[SAND] = sand;
}