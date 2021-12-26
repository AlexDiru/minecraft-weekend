#include "block.h"

static ivec2s get_texture_location(World *world, ivec3s pos, enum Direction d) {
    return (ivec2s) {{ 6, 0 }};
}

void gravel_init() {
    Block gravel = BLOCK_DEFAULT;
    gravel.id = GRAVEL;
    gravel.get_texture_location = get_texture_location;
    BLOCKS[GRAVEL] = gravel;
}