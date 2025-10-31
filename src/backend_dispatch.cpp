#include "backend_dispatch.h"
#include "backend.h"
#include "../backends/cpu/cpu_backend.h"
#include "../backends/metal/metal_backend.h"
#include <iostream>
#include <string>

// ===========================================================
// backend_dispatch.cpp — Unified backend selector
// ===========================================================
//
// This file routes calls to the chosen backend (CPU, Metal, etc.)
// using a common interface so main.cpp doesn’t care which one
// is running under the hood.
//

static BackendAPI make_cpu_backend_api() {
    BackendAPI api;
    api.init     = cpu_init_backend;
    api.step     = cpu_step_backend;
    api.energy   = cpu_compute_backend_energy;
    api.shutdown = cpu_shutdown_backend;
    return api;
}

static BackendAPI make_metal_backend_api() {
    BackendAPI api;
    api.init     = metal_init_backend;
    api.step     = metal_step_backend;
    api.energy   = metal_compute_backend_energy;
    api.shutdown = metal_shutdown_backend;
    return api;
}

// Holds the currently active backend’s API table
static BackendAPI g_backend;

const BackendAPI& get_active_backend() {
    return g_backend;
}

bool select_backend(const std::string& name) {
    if (name == "cpu") {
        g_backend = make_cpu_backend_api();
        std::cout << "[Backend] Selected: CPU\n";
        return true;
    } else if (name == "metal") {
        g_backend = make_metal_backend_api();
        std::cout << "[Backend] Selected: Metal\n";
        return true;
    } else {
        std::cerr << "[Backend] Unknown backend: " << name << "\n";
        return false;
    }
}
