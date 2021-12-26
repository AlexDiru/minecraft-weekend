#include "block.h"

static ivec2s get_texture_location(World *world, ivec3s pos, enum Direction d) {
    return (ivec2s) {{ 3, 2 }};
}

void snow_init() {
    Block snow = BLOCK_DEFAULT;
    snow.id = SNOW;
    snow.get_texture_location = get_texture_location;
    BLOCKS[SNOW] = snow;
}