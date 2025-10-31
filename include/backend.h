#pragma once
#include "types.h"
#include "physics.h"

// ===========================================================
// backend.h — Unified simulation backend interface
// ===========================================================

// Per-backend extra data can live in heap-allocated structs that
// only that backend knows about. We point to it here as void*.
struct BackendContext {
    ParticleArray particles;
    SimParams params;
    int step_count = 0;

    void* backend_data = nullptr; // backend-specific state (Metal, CUDA, etc.)
};

// Common interface every backend must implement:
bool init_backend(BackendContext &ctx, const SimParams &params);
void step_backend(BackendContext &ctx);
float compute_backend_energy(const BackendContext &ctx);
void shutdown_backend(BackendContext &ctx);
