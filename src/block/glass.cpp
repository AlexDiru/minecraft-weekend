#include "block.h"

static ivec2s get_texture_location(World *world, ivec3s pos, enum Direction d) {
    return (ivec2s) {{ 1, 1 }};
}

void glass_init() {
    Block glass = BLOCK_DEFAULT;
    glass.id = GLASS;
    glass.transparent = true;
    glass.get_texture_location = get_texture_location;
    BLOCKS[GLASS] = glass;
}