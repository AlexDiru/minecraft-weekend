#ifndef C_BLOCKLOOK_H
#define C_BLOCKLOOK_H

#include "../util/util.h"
#include "ecs.h"

class BlockLookComponent {
  public:
    bool hit;
    ivec3s pos;
    enum Direction face;
    f32 radius;

    struct {
        bool render : 1;
    } flags;
};

#endif