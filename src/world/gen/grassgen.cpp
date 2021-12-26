#include "worldgen.h"
#include "../world.h"

void worldgen_grass(Chunk *chunk, FWGGet get, FWGSet set, s64 x, s64 y, s64 z) {
    s64 s = RAND(2, 10);
    s64 l = RAND(s - 1, s + 1);
    s64 h = RAND(s - 1, s + 1);

    for (s64 xx = (x - l); xx <= (x + l); xx++) {
        for (s64 zz = (z - h); zz <= (z + h); zz++) {
            BlockId block = static_cast<BlockId>(get(chunk, xx, y + 1, zz));
            BlockId under = static_cast<BlockId>(get(chunk, xx, y, zz));
            if (block == AIR &&
                under == GRASS &&
                RANDCHANCE(0.5)) {
                set(chunk, xx, y + 1, zz, TALLGRASS);
            }
        }
    }
}