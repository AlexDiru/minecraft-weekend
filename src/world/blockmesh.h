#ifndef BLOCKMESH_H
#define BLOCKMESH_H

#include "../util/util.h"

// forward declarations
struct Block;
struct ChunkMesh;

enum BlockMeshType {
    BLOCKMESH_DEFAULT,
    BLOCKMESH_SPRITE,
    BLOCKMESH_LIQUID,
    BLOCKMESH_CUSTOM
};

struct BlockMeshParams {
    struct Block *block, *block_n;
    u64 data, data_n;
    vec2s uv_min, uv_max;
    vec3s position, offset, size;
    enum Direction direction;
};

void blockmesh_sprite(ChunkMesh *mesh, BlockMeshParams params);
void blockmesh_torch(ChunkMesh *mesh, BlockMeshParams params);
void blockmesh_face(ChunkMesh *mesh, BlockMeshParams params);

#endif