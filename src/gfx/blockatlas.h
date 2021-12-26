#ifndef BLOCKATLAS_H
#define BLOCKATLAS_H

#include "../util/util.h"
#include "gfx.h"
#include "texture.h"

#define BLOCK_ATLAS_FPS 6
#define BLOCK_ATLAS_FRAMES 16

struct BlockAtlas {
    ivec2s size, size_sprites, sprite_size;

    Texture frames[BLOCK_ATLAS_FRAMES];
    Atlas atlas;
    size_t ticks;
};

BlockAtlas blockatlas_create(char *path, ivec2s sprite_size);
void blockatlas_destroy(BlockAtlas *self);
void blockatlas_update(BlockAtlas *self);

#endif