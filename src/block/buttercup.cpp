#include "block.h"

static ivec2s get_texture_location(World *world, ivec3s pos, enum Direction d) {
    return (ivec2s) {{ 1, 3 }};
}

void buttercup_init() {
    Block buttercup = BLOCK_DEFAULT;
    buttercup.id = BUTTERCUP;
    buttercup.transparent = true;
    buttercup.solid = false;
    buttercup.mesh_type = BLOCKMESH_SPRITE;
    buttercup.get_texture_location = get_texture_location;
    BLOCKS[BUTTERCUP] = buttercup;
}