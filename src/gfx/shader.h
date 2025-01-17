#ifndef SHADER_H
#define SHADER_H

#include "../util/camera.h"
#include "gfx.h"
#include "texture.h"
#include "../util/util.h"

struct VertexAttr {
    GLuint index;
    const GLchar *name;
};

struct Shader {
    GLuint handle, vs_handle, fs_handle;
};

Shader shader_create(char *vs_path, char *fs_path, size_t n, VertexAttr attributes[]);
void shader_destroy(Shader self);
void shader_bind(Shader self);
void shader_uniform_mat4(Shader self, char *name, mat4s m);
void shader_uniform_view_proj(Shader self, ViewProj view_proj);
void shader_uniform_texture2D(Shader self, char *name, Texture texture, GLuint n);
void shader_uniform_float(Shader self, char *name, f32 f);
void shader_uniform_vec2(Shader self, char *name, vec2s v);
void shader_uniform_vec3(Shader self, char *name, vec3s v);
void shader_uniform_vec4(Shader self, char *name, vec4s v);
void shader_uniform_int(Shader self, char *name, int v);
void shader_uniform_uint(Shader self, char *name, unsigned int v);

#endif