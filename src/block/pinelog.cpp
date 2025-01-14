#include "block.h"

static ivec2s get_texture_location(World *world, ivec3s pos, enum Direction d) {
    switch (d) {
        case UP:
        case DOWN:
            return (ivec2s) {{ 5, 3 }};
        default:
            return (ivec2s) {{ 4, 3 }}; 
    }
}

void pine_log_init() {
    Block pine_log = BLOCK_DEFAULT;
    pine_log.id = PINE_LOG;
    pine_log.get_texture_location = get_texture_location;
    BLOCKS[PINE_LOG] = pine_log;
}