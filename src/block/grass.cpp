#include "block.h"

static ivec2s get_texture_location(World *world, ivec3s pos, enum Direction d) {
    switch (d) {
        case UP:
            return GLMS_IVEC2S_ZERO();
        case DOWN:
            return (ivec2s) {{ 2, 0 }};
        default:
            return (ivec2s) {{ 1, 0 }};
    }
}

void grass_init() {
    Block grass = BLOCK_DEFAULT;
    grass.id = GRASS;
    grass.get_texture_location = get_texture_location;
    BLOCKS[GRASS] = grass;
}