#ifndef SKY_H
#define SKY_H

#include "../gfx/gfx.h"
#include "../util/util.h"
#include "../gfx/vao.h"
#include "../gfx/vbo.h"

// forward declarations
struct World;

enum CelestialBody {
    SUN, MOON
};

enum SkyState {
    DAY = 0, NIGHT, SUNRISE, SUNSET
};

struct Sky {
    World *world;

    // minimum and maximum fog disstances
    f32 fog_near, fog_far;

    // fog and clear colors
    vec4s fog_color, clear_color;

    vec4s sunlight_color;

    enum SkyState state, state_day_night;
    f32 sky_state_progress, day_night_progress;

    VBO ibo, vbo;
    VAO vao;
};

void sky_init(Sky *self, World *world);
void sky_destroy(Sky *self);
void sky_render(Sky *self);

#endif