#ifndef ECS_H
#define ECS_H

#include <vector>

#include "../util/util.h"
#include "ecscomponents.h"
#include "ecstypes.h"


// forward declarations
struct World;
struct CameraComponent;

#define ecs_register(_id, _C, _ecs, _system) _ecs_register_internal((_id), sizeof(_C), (_ecs), (_system))

typedef u64 ECSTag;


#define ECSEVENT_LAST ECS_TICK
enum ECSEvent {
    ECS_INIT = 0, ECS_DESTROY, ECS_RENDER, ECS_UPDATE, ECS_TICK
};

typedef void (*ECSSubscriber)(void *, Entity);

struct ECSSystem {
    struct {
        ECSSubscriber init, destroy, render, update, tick;
    };

    ECSSubscriber subscribers[ECSEVENT_LAST + 1];
};

struct ComponentList {
    ECSSystem system;
    void *components;
    size_t component_size;
};


enum ECSTagValues {
    ECS_TAG_USED = 1 << 0
};

struct ECS {
    ComponentList lists[ECSCOMPONENT_LAST + 1];
    EntityId *ids;
    Bitmap used;
    size_t capacity;
    EntityId next_entity_id;
    World *world;

    ECSComponentManager *componentManager;
};

void _ecs_register_internal(
    ECSComponent id, size_t component_size,
    ECS *ecs, ECSSystem system);

void ecs_event(ECS *self, ECSEvent event);
Entity ecs_new(ECS *self);
void ecs_delete(ECS *self, Entity entity);
void ecs_remove(Entity entity, ECSComponent component);

void ecs_init(ECS *self, World *world);
void ecs_destroy(ECS *self);

// My rewrite

class ECSComponentManager;

class Component {
public:
    Entity* entity;
    ECSComponent id;
    void* value;
    Component(ECSComponentManager* componentManager, Entity* entity, ECSComponent id, void* value) {
        this->entity = entity;
        this->entity->componentManager = componentManager;
        this->id = id;
        this->value = value;
    }
    bool is(Entity* entity, ECSComponent id) const {
        return this->entity == entity && this->id == id;
    }
};

struct BlockLookComponent;

class ECSComponentManager {
    public:
    std::vector<Component> componentVector;

    int getSize() { return componentVector.size(); }

    void add(Entity* entity, ECSComponent component) {
        componentVector.push_back(Component(this, entity, component, NULL));
    }

    void add(Entity* entity, ECSComponent component, void* subEntity) {
        componentVector.push_back(Component(this, entity, component, subEntity));
    }

    void* get(Entity* entity, ECSComponent id) {
      for (int i = 0; i < componentVector.size(); i++) {
          const auto component = componentVector[i];
          if (component.is(entity, id)) {
              return component.value;
          }
      }
      return NULL;
    }

    CameraComponent* getCameraComponent(Entity* entity) {
      void* p = get(entity, C_CAMERA);
      return (CameraComponent*)p;
    }

    PositionComponent* getPositionComponent(Entity* entity) {
        void* p = get(entity, C_POSITION);
        return (PositionComponent*)p;
    }

    ControlComponent*  getControlComponent(Entity* entity) {
        void* p = get(entity, C_CONTROL);
        return (ControlComponent*)p;
    }

    LightComponent*  getLightComponent(Entity* entity) {
        void* p = get(entity, C_LIGHT);
        return (LightComponent*)p;
    }
    BlockLookComponent*  getBlockLookComponent(Entity* entity) {
        void* p = get(entity, C_BLOCKLOOK);
        return (BlockLookComponent*)p;
    }
    MovementComponent*  getMovementComponent(Entity* entity) {
        void* p = get(entity, C_MOVEMENT);
        return (MovementComponent*)p;
    }
    PhysicsComponent*  getPhysicsComponent(Entity* entity) {
        void* p = get(entity, C_PHYSICS);
        return (PhysicsComponent*)p;
    }
    bool ecs_has(Entity* entity, ECSComponent component) {
        return get(entity, component) != NULL;
    }
};

#endif