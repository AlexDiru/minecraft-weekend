#include "noise.h"
#include "../../lib/noise/noise1234.h"

// TODO WHY NOT IMPORTING
float noise3( float x, float y, float z ) {
    return 1.0;
}

f32 octave_compute(void *p_v, f32 seed, f32 x, f32 z) {
    Octave* p = (Octave*)p_v;
    f32 u = 1.0f, v = 0.0f;
    for (int i = 0; i < p->n; i++) {
        v += (1.0f / u) * noise3(float((x / 1.01f) * u), float((z / 1.01f) * u), float(seed + (p->o * 32)));
        u *= 2.0f;
    }
    return v;
}

Noise octave(s32 n, s32 o) {
    Noise result;
    result.compute = (FNoise) octave_compute;
    Octave params = {n, o};
    memcpy(&result.params, &params, sizeof(Octave));
    return result;
}

f32 combined_compute(Combined *p, f32 seed, f32 x, f32 z) {
    return p->n->compute(&p->n->params, seed, x + p->m->compute(&p->m->params, seed, x, z), z);
}

Noise combined(Noise *n, Noise *m) {
    Noise result = {.compute = (FNoise)combined_compute};
    Combined params = {n, m};
    memcpy(&result.params, &params, sizeof(Combined));
    return result;
}

f32 noise_compute(Basic *b, f32 seed, f32 x, f32 z) {
    return noise3(x, z, seed + (b->o * 32.0f));
}

Noise basic(s32 o) {
    Noise result = {.compute = (FNoise) noise_compute };
    Basic params = { .o = o };
    memcpy(&result.params, &params, sizeof(Basic));
    return result;
}

f32 expscale_compute(ExpScale *e, f32 seed, f32 x, f32 z) {
    f32 n = e->n->compute(&e->n->params, seed, x * e->scale, z * e->scale);
    return sign(n) * powf(fabsf(n), e->exp);
}

Noise expscale(Noise *n, float exp, float scale) {
    Noise result = {.compute = (FNoise) expscale_compute };
    ExpScale params = { .n = n, .exp = exp, .scale = scale };
    memcpy(&result.params, &params, sizeof(ExpScale));
    return result;
}