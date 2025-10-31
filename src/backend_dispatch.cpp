#include "backend_dispatch.h"
#include "backend.h"
#include "types.h"
#include "physics.h"

// CPU backend headers
#include "../backends/cpu/cpu_backend.h"
#include "../backends/metal/metal_backend.h"

// (Later) Metal, CUDA headers will go here

#include <iostream>
#include <stdexcept>

// ===========================================================
// backend_dispatch.cpp — Runtime backend selector
// ===========================================================

static BackendAPI g_active_backend = {};
static BackendType g_backend_type = BackendType::CPU;

// -----------------------------------------------------------
// CPU backend registration
// -----------------------------------------------------------
static BackendAPI make_cpu_backend_api() {
    BackendAPI api;
    api.init = init_backend;
    api.step = step_backend;
    api.energy = compute_backend_energy;
    api.shutdown = shutdown_backend;
    return api;
}

// -----------------------------------------------------------
// (Placeholder) Metal backend registration
// -----------------------------------------------------------
static BackendAPI make_metal_backend_api() {
    BackendAPI api;
    api.init = metal_init_backend;
    api.step = metal_step_backend;
    api.energy = metal_compute_backend_energy;
    api.shutdown = metal_shutdown_backend;
    return api;
}

// -----------------------------------------------------------
// (Placeholder) CUDA backend registration
// -----------------------------------------------------------
static BackendAPI make_cuda_backend_api() {
    std::cerr << "[CUDA Backend] Not implemented yet.\n";
    return make_cpu_backend_api(); // fallback to CPU for now
}

// -----------------------------------------------------------
// Select backend by name
// -----------------------------------------------------------
bool select_backend(const std::string& name) {
    if (name == "cpu") {
        g_active_backend = make_cpu_backend_api();
        g_backend_type = BackendType::CPU;
    } else if (name == "metal") {
        g_active_backend = make_metal_backend_api();
        g_backend_type = BackendType::METAL;
    } else if (name == "cuda") {
        g_active_backend = make_cuda_backend_api();
        g_backend_type = BackendType::CUDA;
    } else {
        std::cerr << "Unknown backend: " << name << " (defaulting to CPU)\n";
        g_active_backend = make_cpu_backend_api();
        g_backend_type = BackendType::CPU;
        return false;
    }

    std::cout << "[Backend] Selected: " << name << "\n";
    return true;
}

// -----------------------------------------------------------
// Accessor
// -----------------------------------------------------------
const BackendAPI& get_active_backend() {
    return g_active_backend;
}
