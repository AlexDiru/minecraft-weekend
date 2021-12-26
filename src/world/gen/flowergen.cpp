#include "worldgen.h"
#include "../world.h"

void worldgen_flowers( Chunk *chunk, FWGGet get, FWGSet set, s64 x, s64 y, s64 z) {
    BlockId flower = RANDCHANCE(0.6) ? ROSE : BUTTERCUP;

    s64 s = RAND(2, 6);
    s64 l = RAND(s - 1, s + 1);
    s64 h = RAND(s - 1, s + 1);

    for (s64 xx = (x - l); xx <= (x + l); xx++) {
        for (s64 zz = (z - h); zz <= (z + h); zz++) {
            BlockId block = static_cast<BlockId>(get(chunk, (int)xx, (int)y + 1, (int)zz));
            BlockId under = static_cast<BlockId>(get(chunk, (int)xx, (int)y, (int)zz));
            if (block == AIR &&
                under == GRASS &&
                RANDCHANCE(0.5)) {
                set(chunk, xx, y + 1, zz, flower);
            }
        }
    }
}