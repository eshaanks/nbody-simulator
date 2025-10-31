#pragma once
#include "types.h"
#include "backend.h"

// ===========================================================
// metal_backend.h — Metal GPU Backend (stub version)
// ===========================================================
//
// This header declares the same backend interface functions
// that the CPU version uses, so the dispatcher can call them.
// The implementation will live in metal_backend.mm
//

bool metal_init_backend(BackendContext& ctx, const SimParams& params);
void metal_step_backend(BackendContext& ctx);
float metal_compute_backend_energy(const BackendContext& ctx);
void metal_shutdown_backend(BackendContext& ctx);
