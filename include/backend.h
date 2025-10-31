#pragma once
#include "types.h"
#include "physics.h"

struct BackendContext {
    ParticleArray particles;
    SimParams params;
    int step_count = 0;
};

bool init_backend(BackendContext &ctx, const SimParams &params);
void step_backend(BackendContext &ctx);
float compute_backend_energy(const BackendContext &ctx);
void shutdown_backend(BackendContext &ctx);
