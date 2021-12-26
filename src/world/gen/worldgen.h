#ifndef WORLDGEN_H
#define WORLDGEN_H

#include "../../util/util.h"

// forward declarations
struct Chunk;

typedef void (*FWGSet)(Chunk *, s32, s32, s32, u32);
typedef u32 (*FWGGet)(Chunk *, s32, s32, s32);
typedef void (*FWGDecorate)(Chunk  *, FWGGet, FWGSet, s64, s64, s64);

struct WorldgenData {
    f32 h_b;
    s64 h, b;
};

void worldgen_generate(Chunk *chunk);

void worldgen_pine(Chunk *chunk, FWGGet get, FWGSet set, s64 x, s64 y, s64 z);
void worldgen_tree(Chunk *chunk, FWGGet get, FWGSet set, s64 x, s64 y, s64 z);
void worldgen_flowers(Chunk *chunk, FWGGet get, FWGSet set, s64 x, s64 y, s64 z);
void worldgen_grass(Chunk *chunk, FWGGet get, FWGSet set, s64 x, s64 y, s64 z);
void worldgen_shrub(Chunk *chunk, FWGGet get, FWGSet set, s64 x, s64 y, s64 z);

#endif