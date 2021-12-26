#include "worldgen.h"
#include "../world.h"

void worldgen_shrub(Chunk *chunk, FWGGet get, FWGSet set, s64 x, s64 y, s64 z) {
    BlockId under = static_cast<BlockId>(get(chunk, x, y, z));
    if ((under == SAND || under == DIRT || under == GRASS) &&
        get(chunk, x, y + 1, z) == AIR) {
        set(chunk, x, y + 1, z, SHRUB);
    }
}