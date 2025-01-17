#include "block.h"

static ivec2s get_texture_location(World *world, ivec3s pos, enum Direction d) {
    return (ivec2s) {{ 0, 3 }};
}

void rose_init() {
    Block rose = BLOCK_DEFAULT;
    rose.id = ROSE;
    rose.transparent = true;
    rose.solid = false;
    rose.mesh_type = BLOCKMESH_SPRITE;
    rose.get_texture_location = get_texture_location;
    BLOCKS[ROSE] = rose;
}