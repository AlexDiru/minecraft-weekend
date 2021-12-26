#include "c_blocklook.h"
#include "ecs.h"

#include "../world/world.h"
#include "../state.h"
#include "../gfx/renderer.h"

static bool block_raycast(void* worldPtr, ivec3s pos) {
    World* world = (World*)worldPtr;
    Block block = BLOCKS[world_get_block(world, pos)];
    return block.id != AIR && !block.liquid;
}

static void tick(BlockLookComponent *c_blocklook, Entity* entity) {
    CameraComponent *c_camera = entity->componentManager->getCameraComponent(entity);

    ivec3s pos;
    int dir;

    Ray ray;
    ray.origin = c_camera->camera.position;
    ray.direction = c_camera->camera.direction;

    if (ray_block(ray, c_blocklook->radius, entity->ecs->world, block_raycast, &pos, &dir)) {
        c_blocklook->hit = true;
        c_blocklook->pos = pos;
        c_blocklook->face = static_cast<Direction>(dir);
    } else {
        c_blocklook->hit = false;
    }
}

static void render(BlockLookComponent *c_blocklook, Entity* entity) {
    if (c_blocklook->flags.render && c_blocklook->hit) {
        AABB aabb;
        BLOCKS[world_get_block(entity->ecs->world, c_blocklook->pos)]
            .get_aabb(entity->ecs->world, c_blocklook->pos, aabb);
        glms_aabb_scale(aabb, (vec3s) {{ 1.005f, 1.005f, 1.005f }}, aabb);
        renderer_aabb(
            &state.renderer, aabb,
            (vec4s) {{ 1.0f, 1.0f, 1.0f,
                ((state.ticks % 40) > 20 ?
                    ((state.ticks % 40) / 40.0f) :
                    (1.0f - ((state.ticks % 40) / 40.0f))) * 0.3f }},
            glms_mat4_identity(),
            FILL_MODE_FILL);
    }
}

void c_blocklook_init(ECS *ecs) {
    ecs_register(C_BLOCKLOOK, BlockLookComponent, ecs, ((ECSSystem) {
        .init = NULL,
        .destroy = NULL,
        .render = (ECSSubscriber) render,
        .update = NULL,
        .tick = (ECSSubscriber) tick
    }));
}