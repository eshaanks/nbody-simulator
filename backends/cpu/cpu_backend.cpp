#include "backend.h"
#include <iostream>
#include <random>
#include <chrono>

// ===========================================================
// cpu_backend.cpp — CPU implementation of the simulation backend
// ===========================================================

// Utility to generate random floats in range [-1, 1]
static float frand() {
    static std::mt19937 gen(
        static_cast<unsigned long>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    return dist(gen);
}

// -----------------------------------------------------------
// Initialize the backend
// -----------------------------------------------------------
bool init_backend(BackendContext &ctx, const SimParams &params) {
    ctx.params = params;
    ctx.step_count = 0;
    ctx.particles.resize(params.N);

    for (auto &p : ctx.particles) {
        p.x = frand();
        p.y = frand();
        p.z = frand();
        p.vx = 0.1f * frand();
        p.vy = 0.1f * frand();
        p.vz = 0.1f * frand();
        p.mass = 1.0f;
    }

    compute_accelerations(ctx.particles);
    std::cout << "[CPU Backend] Initialized with " << params.N << " particles.\n";
    return true;
}

// -----------------------------------------------------------
// Advance one timestep using Leapfrog integration
// -----------------------------------------------------------
void step_backend(BackendContext &ctx) {
    integrate_leapfrog(ctx.particles, ctx.params.dt);
    ctx.step_count++;
}

// -----------------------------------------------------------
// Compute total system energy
// -----------------------------------------------------------
float compute_backend_energy(const BackendContext &ctx) {
    return compute_total_energy(ctx.particles);
}

// -----------------------------------------------------------
// Shutdown backend (no-op for CPU)
// -----------------------------------------------------------
void shutdown_backend(BackendContext &ctx) {
    ctx.particles.clear();
    std::cout << "[CPU Backend] Shutdown complete.\n";
}
