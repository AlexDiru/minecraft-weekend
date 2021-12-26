#ifndef NOISE_H
#define NOISE_H

#include "../../util/util.h"

typedef f32 (*FNoise)(void *p, f32 s, f32 x, f32 z);

struct Noise {
    u8 params[512];
    FNoise compute;
};

// Octave noise with n octaves and seed offset o
// Maximum amplitude is 2^0 + 2^1 + 2^2 ... 2^n = 2^(n+1) - 1
// i.e. for octave 8, values range between [-511, 511]
struct Octave {
    s32 n, o;
};

// Combined noise where compute(x, z) = n.compute(x + m.compute(x, z), z)
struct Combined {
    Noise *n, *m;
};

struct Basic {
    s32 o;
};

struct ExpScale {
    Noise *n;
    f32 exp, scale;
};

Noise octave(s32 n, s32 o);
Noise combined(Noise *n, Noise *m);
Noise basic(s32 o);
Noise expscale(Noise *n, float exp, float scale);

#endif