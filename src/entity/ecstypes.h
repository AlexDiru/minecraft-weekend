#ifndef ECSTYPES_H
#define ECSTYPES_H

#include "../util/util.h"

#define ENTITY_NONE 0

typedef u64 EntityId;

struct ECS;
class ECSComponentManager;

struct Entity {
    EntityId id;
    u64 index;
    ECS *ecs;
    ECSComponentManager* componentManager;
};

#endif