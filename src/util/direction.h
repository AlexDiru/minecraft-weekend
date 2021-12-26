#ifndef DIRECTION_H
#define DIRECTION_H

#include <cglm/cglm.h>
#include <cglm/struct.h>
#include "fmath.h"

enum Direction {
    NORTH = 0,
    SOUTH = 1,
    EAST = 2,
    WEST = 3,
    UP = 4,
    DOWN = 5
};

extern const ivec3s DIRECTION_IVEC[6];
extern const vec3s DIRECTION_VEC[6];

Direction _ivec3s2dir(ivec3s v);

#define DIR2VEC3S(d) (DIRECTION_VEC[d])
#define DIR2IVEC3S(d) (DIRECTION_IVEC[d])
#define IVEC3S2DIR(v) (_ivec3s2dir(v))



static ivec3s directionIndexToIVec3s(int directionIndex) {
    vec3s directionVec = DIRECTION_VEC[directionIndex];

    // for some reason VEC3S2I doesn't work...
    ivec3s i;
    i.x = (int)directionVec.x;
    i.y = (int)directionVec.y;
    i.z = (int)directionVec.z;
    return i;
}
static vec3s directionIndexToVec3s(int directionIndex) {
    return DIRECTION_VEC[directionIndex];
}

static Direction directionIndexToDirection(int directionIndex) {
    // Maybe there's a better way to do this, cba until I know this works
    switch (directionIndex) {
    case 0:
        return NORTH;
    case 1:
        return SOUTH;
    case 2:
        return EAST;
    case 3:
        return WEST;
    case 4:
        return UP;
    case 5:
        return DOWN;
    }
}

#endif