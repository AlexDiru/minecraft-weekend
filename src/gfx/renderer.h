#ifndef RENDERER_H
#define RENDERER_H

#include "../util/util.h"
#include "blockatlas.h"
#include "gfx.h"
#include "shader.h"
#include "vao.h"
#include "vbo.h"

enum FillMode {
    FILL_MODE_FILL, FILL_MODE_LINE
};

#define SHADERS_LAST SHADER_BASIC_COLOR
enum ShaderType {
    SHADER_NONE = 0,
    SHADER_CHUNK,
    SHADER_SKY,
    SHADER_BASIC_TEXTURE,
    SHADER_BASIC_COLOR
};

#define TEXTURE_LAST TEXTURE_HOTBAR
enum Textures {
    TEXTURE_CROSSHAIR,
    TEXTURE_CLOUDS,
    TEXTURE_STAR,
    TEXTURE_SUN,
    TEXTURE_MOON,
    TEXTURE_HOTBAR
};

#define CAMERA_STACK_MAX 256

enum RenderPass {
    PASS_2D,
    PASS_3D
};

struct Renderer {
    enum CameraType camera_type;
    struct {
        enum CameraType array[CAMERA_STACK_MAX];
        size_t size;
    } camera_stack;

    struct PerspectiveCamera perspective_camera;
    struct OrthoCamera ortho_camera;

    struct Shader shaders[SHADERS_LAST + 1];
    enum ShaderType current_shader;
    struct Shader shader;

    struct Texture textures[TEXTURE_LAST + 1];

    struct BlockAtlas block_atlas;

    vec4s clear_color;

    struct VBO vbo, ibo;
    struct VAO vao;

    struct {
        bool wireframe : 1;
    } flags;
};

void renderer_init(Renderer *self);
void renderer_destroy(Renderer *self);
void renderer_update(Renderer *self);
void renderer_prepare(Renderer *self, enum RenderPass pass);
void renderer_set_camera(Renderer *self, enum CameraType type);
void renderer_push_camera(Renderer *self);
void renderer_pop_camera(Renderer *self);
void renderer_set_view_proj(Renderer *self);
void renderer_use_shader(Renderer *self, enum ShaderType shader);

void renderer_quad_color(
    Renderer *self, vec2s size,
    vec4s color, mat4s model);

void renderer_quad_texture(
    Renderer *self, Texture texture,
    vec2s size, vec4s color,
    vec2s uv_min, vec2s uv_max,
    mat4s model);

void renderer_aabb(
    Renderer *self, AABB aabb, vec4s color,
    mat4s model, enum FillMode fill_mode);

#endif