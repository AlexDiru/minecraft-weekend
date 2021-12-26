#include "../state.h"
#include "world.h"
#include "light.h"
#include "../util/sort.h"

// the total number of chunks in _w->chunks
#define NUM_CHUNKS(_w) ((_w)->chunks_size * (_w)->chunks_size * (_w)->chunks_size)

// the total number of heightmaps in _w->heightmaps
#define NUM_HEIGHTMAPS(_w) ((_w)->chunks_size * (_w)->chunks_size)

#define world_foreach(_w, _cname)\
    struct Chunk *_cname;\
    for (size_t i = 0; i < NUM_CHUNKS(_w) &&\
        (_cname = (_w)->chunks[i]) != (void *) INT64_MAX;\
        i++)

// back-to-front ordering pointer comparator
static int _btf_pcmp(const ivec3s **a, const ivec3s **b, ivec3s *center) {
    return -(glms_ivec3_norm2(glms_ivec3_sub(*center, **a)) - glms_ivec3_norm2(glms_ivec3_sub(*center, **b)));
}

// front-to-back ordering pointer comparator
static int _ftb_pcmp(const ivec3s **a, const ivec3s **b, ivec3s *center) {
    return glms_ivec3_norm2(glms_ivec3_sub(*center, **a)) - glms_ivec3_norm2(glms_ivec3_sub(*center, **b));
}

// back-to-front ordering comparator
static int _btf_cmp(const ivec3s *a, const ivec3s *b, ivec3s *center) {
    return -(glms_ivec3_norm2(glms_ivec3_sub(*center, *a)) - glms_ivec3_norm2(glms_ivec3_sub(*center, *b)));
}

// front-to-back ordering comparator
static int _ftb_cmp(const ivec3s *a, const ivec3s *b, ivec3s *center) {
    return -(glms_ivec3_norm2(glms_ivec3_sub(*center, *b)) - glms_ivec3_norm2(glms_ivec3_sub(*center, *a)));
}

// _cmp is comparison function for qsort_r
// _v0 is 'n'
// _v1 is counter
// _v2 is ivec3s* offsets array
#define _world_foreach_cmp_impl(_w, _cname, _cmp, _v0, _v1, _v2)\
    size_t _v0 = 0, _v1 = 0;\
    ivec3s *_v2[NUM_CHUNKS(_w)];\
    for (_v1 = 0; _v1 < NUM_CHUNKS(_w); _v1++)\
        { if (_w->chunks[_v1] != NULL) _v2[_v0++] = &_w->chunks[_v1]->offset; }\
    sort_r(_v2, _v0, sizeof(ivec3s*), (int (*)(const void*, const void*, void*)) _cmp, &_w->center_offset);\
    struct Chunk *_cname;\
    for (size_t i = 0; i < _v0 &&\
        (_cname = (_w)->chunks[world_chunk_index(_w, *_v2[i])]) != (void *) INT64_MAX;\
        i++)

#define _world_foreach_offset_cmp_impl(_w, _iname, _oname, _cmp, _v0, _v1, _v2)\
    size_t _v0 = NUM_CHUNKS(_w), _v1 = 0, _iname = 0;\
    ivec3s _v2[_v0];\
    for (_v1 = 0; _v1 < _v0; _v1++)\
        { _v2[_v1] = world_chunk_offset(_w, _v1); }\
    sort_r(_v2, _v0, sizeof(ivec3s), (int (*)(const void*, const void*, void*)) _cmp, &_w->center_offset);\
    ivec3s _oname;\
    for (_v1 = 0; _v1 < _v0 &&\
        ((_iname = world_chunk_index(_w, _v2[_v1])) != (size_t) - 1 &&\
        (_oname.x = _v2[_v1].x) != INT32_MIN &&\
        (_oname.y = _v2[_v1].y) != INT32_MIN &&\
        (_oname.z = _v2[_v1].z) != INT32_MIN);\
        _v1++)

#define CONCAT_IMPL(x, y) x ## y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

// iterate chunks from the borders in (back to front)
#define world_foreach_btf(_w, _cname)\
    _world_foreach_cmp_impl(_w, _cname, _btf_pcmp, CONCAT(v, __COUNTER__), CONCAT(v, __COUNTER__), CONCAT(v, __COUNTER__))
    
// iterate chunks from the center out (front to back)
#define world_foreach_ftb(_w, _cname)\
    _world_foreach_cmp_impl(_w, _cname, _ftb_pcmp, CONCAT(v, __COUNTER__), CONCAT(v, __COUNTER__), CONCAT(v, __COUNTER__))

// iterate OFFSETS from the borders in (back to front)
#define world_foreach_offset_btf(_w, _iname, _oname)\
    _world_foreach_offset_cmp_impl(_w, _iname, _oname, _btf_cmp, CONCAT(v, __COUNTER__), CONCAT(v, __COUNTER__), CONCAT(v, __COUNTER__))

// iterate OFFSETS from the center out (front to back)
#define world_foreach_offset_ftb(_w, _iname, _oname)\
    _world_foreach_offset_cmp_impl(_w, _iname, _oname, _ftb_cmp, CONCAT(v, __COUNTER__), CONCAT(v, __COUNTER__), CONCAT(v, __COUNTER__))

// world block position -> heightmap internal position
static inline ivec2s world_pos_to_heightmap_pos(ivec2s pos) {
    return glms_ivec2_mod(glms_ivec2_add(glms_ivec2_mod(pos, CHUNK_SIZE_XZ), CHUNK_SIZE_XZ), CHUNK_SIZE_XZ);
}

static inline bool world_heightmap_in_bounds(World *self, ivec2s offset) {
    ivec2s p = glms_ivec2_sub(offset, (ivec2s) {{ self->chunks_origin.x, self->chunks_origin.z }});
    return p.x >= 0 && p.y >= 0 && p.x < (s32) self->chunks_size && p.y < (s32) self->chunks_size;
}

// heightmap offset -> array index
static inline size_t world_heightmap_index(World *self, ivec2s offset) {
    ivec2s p = glms_ivec2_sub(offset, (ivec2s) {{ self->chunks_origin.x, self->chunks_origin.z }});
    return (p.x * self->chunks_size) + p.y;
}

// array index -> heightmap offset
static inline ivec2s world_heightmap_offset(World *self, size_t i) {
    return (ivec2s) {{
        (int)(self->chunks_origin.x + (i / self->chunks_size)),
        (int)(self->chunks_origin.z + (i % self->chunks_size))
    }};
}

// returns the heightmap for the specified chunk
Heightmap* chunk_get_heightmap(Chunk *self) {
    World* world = self->world;
    Heightmap** heightmaps = world->heightmaps;
    int index = world_heightmap_index(world, { self->offset.x, self->offset.z });

    return heightmaps[index];
}

// returns heightmap value at specified x, z coordinate
s64 world_heightmap_get(World *self, ivec2s p) {
    ivec3s offset_c = world_pos_to_offset({ p.x, 0, p.y });
    ivec2s offset_h = { offset_c.x, offset_c.z };

    if (world_heightmap_in_bounds(self, offset_h)) {
        return HEIGHTMAP_GET(
            self->heightmaps[world_heightmap_index(self, offset_h)],
            world_pos_to_heightmap_pos(p));
    }

    return HEIGHTMAP_UNKNOWN;
}

// sets heightmap value at specified x, z coordinate
void world_heightmap_set(World *self, ivec2s p, s64 y) {
    ivec3s offset = world_pos_to_offset((ivec3s) {{ p.x, 0, p.y }});
    HEIGHTMAP_SET(self->heightmaps[
            world_heightmap_index(self, (ivec2s) {{ offset.x, offset.z }})],
        world_pos_to_heightmap_pos(p), y);
}

// updates heightmap to be max(p.y, current heightmap value at (p.x, p.z))
// returns true if heighmap value changed
bool world_heightmap_update(World *self, ivec3s p) {
    ivec3s offset = world_pos_to_offset(p);
    Heightmap *h = self->heightmaps[
           world_heightmap_index(self, (ivec2s) {{ offset.x, offset.z }})
        ];
    size_t i = HEIGHTMAP_INDEX(world_pos_to_heightmap_pos((ivec2s) {{ p.x, p.z }}));

    if (p.y > h->data[i]) {
        h->data[i] = p.y;
        return true;
    }

    return false;
}

// recalculate heightmap values for an entire chunk
void chunk_heightmap_recalculate(Chunk *chunk) {
    Heightmap *heightmap = chunk_get_heightmap(chunk);
    ivec3s pos_c, pos_w;

    for (s64 x = 0; x < CHUNK_SIZE.x; x++) {
        for (s64 z = 0; z < CHUNK_SIZE.z; z++) {
            pos_c.x = x;
            pos_c.z = z;
            pos_w.x = x + chunk->position.x;
            pos_w.z = z + chunk->position.z;

            s64 h = HEIGHTMAP_GET(heightmap, ((ivec2s) {{ (int)x, (int)z }}));

            // don't set heightmap value if it's already above this column
            if (h > (chunk->position.y + CHUNK_SIZE.y - 1)) {
                continue;
            }

            for (s64 y = CHUNK_SIZE.y - 1; y >= 0; y--) {
                pos_c.y = y;
                pos_w.y = y + chunk->position.y;

                if (!BLOCKS[chunk_get_block(chunk, pos_c)].transparent) {
                    HEIGHTMAP_SET(heightmap, ((ivec2s) {{ pos_c.x, pos_c.z }}), pos_w.y);
                    break;
                }
            }
        }
    }
}

// recalculate the heightmap value at the specified x, z coordinate
void world_heightmap_recalculate(World *self, ivec2s p) {
    assert(world_in_bounds(self, (ivec3s) {{ p.x, 0, p.y }}));
    s64 y_min = self->chunks_origin.y * CHUNK_SIZE.y,
        y_max = (self->chunks_origin.y + self->chunks_size) * CHUNK_SIZE.y;

    for (s64 y = y_max; y >= y_min; y--) {
        ivec3s w = (ivec3s) {{ (int)p.x, (int)y, (int)p.y }};
        if (!BLOCKS[world_get_block(self, w)].transparent) {
            world_heightmap_set(self, p, y);
            return;
        }
    }

    world_heightmap_set(self, p, HEIGHTMAP_UNKNOWN);
}

void world_load_chunk(World *self, ivec3s offset) {
    assert(!world_contains_chunk(self, offset));
    Chunk *chunk = (Chunk*)malloc(sizeof(Chunk));
    chunk_init(chunk, self, offset);
    chunk->flags.generating = true;
    worldgen_generate(chunk);

    // set blocks which were previously unloaded
    for (size_t i = 0; i < self->unloaded_blocks.size; i++) {
        WorldUnloadedBlock data = self->unloaded_blocks.list[i];
        if (!ivec3scmp(chunk->offset, world_pos_to_offset(data.pos))) {
            chunk_set_block(chunk, world_pos_to_chunk_pos(data.pos), data.block);
            world_remove_unloaded_block(self, i);
        }
    }

    chunk->flags.generating = false;

    self->chunks[world_chunk_index(self, chunk->offset)] = chunk;
    chunk_after_generate(chunk);
}

void world_init(World *self) {
    ecs_init(&self->ecs, self);
    sky_init(&self->sky, self);

    self->ticks = 0;

    SRAND(NOW());
    self->seed = RAND(0, ULONG_MAX - 1);

    self->throttles.load.max = 2;
    self->throttles.mesh.max = 8;

    self->unloaded_blocks.capacity = 64;
    self->unloaded_blocks.list = (WorldUnloadedBlock*)malloc(self->unloaded_blocks.capacity * sizeof(struct WorldUnloadedBlock));

    self->chunks_size = 16;
    self->chunks = (Chunk**)calloc(1, NUM_CHUNKS(self) * sizeof(struct Chunk *));
    self->heightmaps = (Heightmap**)calloc(1, NUM_HEIGHTMAPS(self) * sizeof(struct Heightmap *));

    world_set_center(self, { 0, 0, 0 });
}

void world_destroy(World *self) {
    // destroy remaining entitites
    ecs_event(&self->ecs, ECS_DESTROY);
    ecs_destroy(&self->ecs);

    sky_destroy(&self->sky);

    world_foreach(self, chunk) {
        if (chunk != NULL) {
            chunk_destroy(chunk);
            free(chunk);
        }
    }

    free(self->chunks);
    free(self->heightmaps);
}

void world_append_unloaded_block(World *self, ivec3s pos, enum BlockId block) {
    if (self->unloaded_blocks.size + 1 == self->unloaded_blocks.capacity) {
        self->unloaded_blocks.capacity *= 2;
        self->unloaded_blocks.list = (WorldUnloadedBlock*)realloc(
            self->unloaded_blocks.list,
            self->unloaded_blocks.capacity * sizeof(WorldUnloadedBlock));
    }

    self->unloaded_blocks.list[self->unloaded_blocks.size++] = (WorldUnloadedBlock) {
        .pos = pos,
        .block = block
    };
}

void world_remove_unloaded_block(World *self, size_t i) {
    assert(i >= 0 && i < self->unloaded_blocks.size);
    self->unloaded_blocks.size--;

    if (i != self->unloaded_blocks.size) {
        memmove(
            self->unloaded_blocks.list + (i + 0),
            self->unloaded_blocks.list + (i + 1),
            (self->unloaded_blocks.size - i) * sizeof(WorldUnloadedBlock));
    }
}

// Attempt to load any NULL chunks
static void load_empty_chunks(World *self) {
    world_foreach_offset_ftb(self, i, offset) {
        if (self->chunks[i] == NULL &&
            self->throttles.load.count < self->throttles.load.max) {
            world_load_chunk(self, world_chunk_offset(self, i));
            self->throttles.load.count++;
        }
    }
}

// Centers the world's loaded chunks around the specified block position
void world_set_center(World *self, ivec3s center_pos) {
    ivec3s new_offset = world_pos_to_offset(center_pos);
    ivec3s new_origin = glms_ivec3_sub(new_offset, (ivec3s) {{
        (int)(self->chunks_size / 2),
        (int)(self->chunks_size / 2),
        (int)(self->chunks_size / 2)
    }});

    if (!ivec3scmp(new_origin, self->chunks_origin)) {
        // Do nothing if the center chunk hasn't moved
        return;
    }

    // Re-center
    self->center_offset = new_offset;
    self->chunks_origin = new_origin;

    // Backup current chunks
    Chunk *old[NUM_CHUNKS(self)];
    memcpy(old, self->chunks, NUM_CHUNKS(self) * sizeof(Chunk *));

    // Set world to all unloaded chunks initially
    memset(self->chunks, 0, NUM_CHUNKS(self) * sizeof(Chunk *));

    // Place old chunks in positions of new chunk array, destroy if they are out of bounds now
    for (size_t i = 0; i < NUM_CHUNKS(self); i++) {
        Chunk *c = old[i];
        if (c == NULL) {
            continue;
        } else if (world_chunk_in_bounds(self, c->offset)) {
            self->chunks[world_chunk_index(self, c->offset)] = c;
        } else {
            chunk_destroy(c);
            free(c);
        }
    }

    // Move heightmaps
    struct Heightmap *old_heightmaps[NUM_HEIGHTMAPS(self)];
    memcpy(old_heightmaps, self->heightmaps, NUM_HEIGHTMAPS(self) * sizeof(Heightmap *));
    memset(self->heightmaps, 0, NUM_HEIGHTMAPS(self) * sizeof(Heightmap *));

    for (size_t i = 0; i < NUM_HEIGHTMAPS(self); i++) {
        struct Heightmap *h = old_heightmaps[i];
        if (h == NULL) {
            continue;
        } else if (world_heightmap_in_bounds(self, h->offset)) {
            self->heightmaps[world_heightmap_index(self, h->offset)] = h;
        } else {
            free(h->worldgen_data);
            free(h->data);
            free(h);
        }
    }

    // Create empty heightmaps
    for (size_t i = 0; i < NUM_HEIGHTMAPS(self); i++) {
        if (self->heightmaps[i] == NULL) {
            self->heightmaps[i] = (Heightmap*)calloc(1, sizeof(struct Heightmap));
            self->heightmaps[i]->offset = world_heightmap_offset(self, i);
            self->heightmaps[i]->data = (s64*)malloc(CHUNK_SIZE.x * CHUNK_SIZE.z * sizeof(s64));
            self->heightmaps[i]->worldgen_data = (WorldgenData*)malloc(CHUNK_SIZE.x * CHUNK_SIZE.z * sizeof(WorldgenData));
            memsetl(self->heightmaps[i]->data, HEIGHTMAP_UNKNOWN, CHUNK_SIZE.x * CHUNK_SIZE.z * sizeof(s64));
        }
    }

    load_empty_chunks(self);
}

void world_render(World *self) {
    // copy camera settings from view entity
    CameraComponent *c_camera = self->componentManager->getCameraComponent(self->entity_view);
    memcpy(&state.renderer.perspective_camera, &c_camera->camera, sizeof(struct PerspectiveCamera));

    // configure sky
    self->sky.fog_near = ((int)self->chunks_size / 2) * 32 - 12;
    self->sky.fog_far = ((int)self->chunks_size / 2) * 32 - 4;

    sky_render(&self->sky);
    state.renderer.clear_color = self->sky.clear_color;

    // prepare (mesh) chunks from nearest to farthest
    world_foreach_ftb(self, c0) {
        if (c0 != NULL) {
            chunk_prepare(c0);
        } 
    }

    renderer_use_shader(&state.renderer, SHADER_CHUNK);
    renderer_push_camera(&state.renderer);
    renderer_set_camera(&state.renderer, CAMERA_PERSPECTIVE);
    renderer_set_view_proj(&state.renderer);
    shader_uniform_texture2D(state.renderer.shaders[SHADER_CHUNK], "tex", state.renderer.block_atlas.atlas.texture, 0);
    shader_uniform_vec4(state.renderer.shaders[SHADER_CHUNK], "sunlight_color", self->sky.sunlight_color);

    shader_uniform_vec4(state.renderer.shaders[SHADER_CHUNK], "fog_color", self->sky.fog_color);
    shader_uniform_float(state.renderer.shaders[SHADER_CHUNK], "fog_near", self->sky.fog_near);
    shader_uniform_float(state.renderer.shaders[SHADER_CHUNK], "fog_far", self->sky.fog_far);
    

    // render solid geometry in no particular order
    world_foreach(self, c1) {
        if (c1 != NULL) {
            chunk_render(c1, BASE);
        }
    }

    // render transparent geometry back to front
    glDisable(GL_CULL_FACE);
    world_foreach_btf(self, c2) {
        if (c2 != NULL) {
            chunk_render(c2, TRANSPARENT);
        }
    }
    glEnable(GL_CULL_FACE);

    ecs_event(&self->ecs, ECS_RENDER);
    renderer_pop_camera(&state.renderer);
}

void world_update(World *self) {
    // reset throttles
    self->throttles.load.count = 0;
    self->throttles.mesh.count = 0;

    load_empty_chunks(self);

    world_foreach(self, chunk) {
        if (chunk != NULL) {
            chunk_update(chunk);
        }
    }
    
    ecs_event(&self->ecs, ECS_UPDATE);
}

void world_tick(World *self) {
    self->ticks++;

    world_set_center(self, (self->componentManager->getPositionComponent(self->entity_view))->block);

    world_foreach(self, chunk) {
        if (chunk != NULL) {
            chunk_tick(chunk);
        }
    }

    ecs_event(&self->ecs, ECS_TICK);
}

// returns all AABBs within the specified area
size_t world_get_aabbs(World *self, AABB area, AABB *aabbs, size_t n) {
    ivec3s b_min = world_pos_to_block(area[0]),
        b_max = glms_ivec3_add(world_pos_to_block(area[1]), GLMS_IVEC3_ONE);

    size_t i = 0;
    for (s64 x = b_min.x; x <= b_max.x; x++) {
        for (s64 z = b_min.z; z <= b_max.z; z++) {
            for (s64 y = b_min.y; y <= b_max.y; y++) {
                ivec3s pos = (ivec3s) {{ (int)x, (int)y, (int)z }};
                struct Block block = BLOCKS[world_get_block(self, pos)];
                
                if (block.solid) {
                    block.get_aabb(self, pos, aabbs[i++]);

                    if (i == n) {
                        // TODO: real warning here?
                        printf("%s", "WARNING: AABB buffer filled\n");
                        break;
                    }
                }
            }
        }
    }

    return i;
}