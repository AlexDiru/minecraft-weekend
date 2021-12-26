#include "block.h"

static ivec2s get_texture_location(World *world, ivec3s pos, enum Direction d) {
    return (ivec2s) {{ 4, 0 }};
}

void coal_init() {
    Block coal = BLOCK_DEFAULT;
    coal.id = COAL;
    coal.get_texture_location = get_texture_location;
    BLOCKS[COAL] = coal;
}