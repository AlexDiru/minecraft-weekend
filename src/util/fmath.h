#ifndef FMATH_H
#define FMATH_H

#include <cglm/cglm.h>
#include <cglm/struct.h>

#include "types.h"
#include "direction.h"
#include "ivec2s.h"

static vec2s IVEC2S2V(ivec2s _v) {
    ivec2s _u = ( _v );
    vec2s v;
    v.x = _u.x;
    v.y = _u.y;
    return v;
}

static ivec2s VEC2S2I(vec2s _v) {
    ivec2s v;
    v.x = (int)_v.x;
    v.y = (int)_v.y;
    return v;
}

static vec3s IVEC3S2V(ivec3s _v) {
    ivec3s _u = ( _v );
    vec3s v;
    v.x = (int)_u.x;
    v.y = (int)_u.y;
    v.z = (int)_u.z;
    return v;
}


static ivec3s VEC3S2I(vec3s _v) {
    ivec3s v;
    v.x = (int)_v.x;
    v.y = (int)_v.y;
    v.z = (int)_v.z;
    return v;
}

#define IVEC2S23(_v) ({ ivec2s _u = ( _v ); ((ivec3s) {{ _u.x, _u.y, 0 }}); })

#define PI_2 GLM_PI_2
#define PI GLM_PI
#define TAU (2 * PI)
#define F32_EPSILON FLT_EPSILON
#define F64_EPSILON DBL_EPSILON

#define cosf(_x) ((f32)cos(_x))
#define sinf(_x) ((f32)sin(_x))
#define radians(_x) ((_x) * (__typeof__(_x)) (PI / 180.0f))
#define degrees(_x) ((_x) * (__typeof__(_x)) (180.0f / PI))
#define floori(_x) ((int) (floor((double) (_x))))
#define sign(_x) ({ __typeof__(_x) _xx = (_x);\
    ((__typeof__(_x)) ( (((__typeof__(_x)) 0) < _xx) - (_xx < ((__typeof__(_x)) 0))));})
#define lerpf(_a, _b, _t) ({ __typeof__(_t) _u = (_t); ((_a) * (1 - _u)) + ((_b) * _u); })
#define safe_expf(_x, _e) ({ __typeof__(_x) __x = (_x); __typeof__(_e) __e = (_e); sign(__x) * fabsf(powf(fabsf(__x), __e)); })

#define max(a, b) ({\
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a, b) ({\
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })


#define SRAND(seed) srand(seed)
#define RAND(min, max) ((rand() % (max - min + 1)) + min)
#define RANDCHANCE(chance) ((RAND(0, 100000) / 100000.0) <= chance)

int ivec3scmp(ivec3s a, ivec3s b);
s64 ivec3shash(ivec3s v);

struct Ray {
    vec3s origin, direction;
};

extern bool ray_block(Ray ray, f32 max_distance, void *arg, bool (*f)(void *, ivec3s), ivec3s *out, int *d_out);

#endif