#pragma once
#include "types.h"
#include "backend.h"

// ===========================================================
// cpu_backend.h — CPU Reference Backend
// ===========================================================
//
// Provides CPU implementation of the simulation backend
// using the same interface as Metal or CUDA versions.
// This allows the dispatcher to call any backend interchangeably.
//

bool cpu_init_backend(BackendContext& ctx, const SimParams& params);
void cpu_step_backend(BackendContext& ctx);
float cpu_compute_backend_energy(const BackendContext& ctx);
void cpu_shutdown_backend(BackendContext& ctx);
