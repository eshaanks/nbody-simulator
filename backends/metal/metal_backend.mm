#import <Metal/Metal.h>
#include "metal_backend.h"
#include <iostream>
#include <random>
#include <chrono>

// We'll access BackendContext, SimParams, Particle, etc.
#include "backend.h"
#include "types.h"

// --------------------------------------------
// Internal per-backend state for Metal
// --------------------------------------------
struct MetalState {
    id<MTLDevice> device;
    id<MTLCommandQueue> commandQueue;

    id<MTLBuffer> particleBuffer;

    uint32_t numParticles;
};

// helper: random float in [-1, 1]
static float frand() {
    static std::mt19937 gen(
        static_cast<unsigned long>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    return dist(gen);
}

// -----------------------------------------------------------
// metal_init_backend
// -----------------------------------------------------------
bool metal_init_backend(BackendContext& ctx, const SimParams& params) {
    // 1. get GPU device
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    if (!device) {
        std::cerr << "[Metal] ERROR: No Metal device found.\n";
        return false;
    }

    // 2. create command queue
    id<MTLCommandQueue> queue = [device newCommandQueue];
    if (!queue) {
        std::cerr << "[Metal] ERROR: Failed to create command queue.\n";
        return false;
    }

    // 3. allocate and initialize particle data on CPU first
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
        // ax/ay/az can start at 0; we'll compute later on GPU
        p.ax = p.ay = p.az = 0.0f;
    }

    // 4. create a GPU buffer to hold particles
    // NOTE: ctx.particles is a std::vector<Particle>
    // We want a Metal buffer with same bytes.
    NSUInteger bufferSize = ctx.particles.size() * sizeof(Particle);

    id<MTLBuffer> particleBuffer =
        [device newBufferWithBytes:ctx.particles.data()
                            length:bufferSize
                           options:MTLResourceStorageModeManaged];

    if (!particleBuffer) {
        std::cerr << "[Metal] ERROR: Failed to create particle buffer.\n";
        return false;
    }

    // 5. allocate MetalState and stash in ctx.backend_data
    MetalState* state = new MetalState();
    state->device         = device;
    state->commandQueue   = queue;
    state->particleBuffer = particleBuffer;
    state->numParticles   = (uint32_t)ctx.particles.size();

    ctx.backend_data = state;

    std::cout << "[Metal Backend] init: "
              << state->numParticles
              << " particles on GPU '"
              << [[device name] UTF8String]
              << "'\n";

    return true;
}

// -----------------------------------------------------------
// metal_step_backend
// -----------------------------------------------------------
//
// Right now: just a stub.
// Eventually this will:
//  - encode a command buffer
//  - run compute kernels for acceleration + integration
//
void metal_step_backend(BackendContext& ctx) {
    MetalState* state = reinterpret_cast<MetalState*>(ctx.backend_data);
    (void)state; // silence unused warning for now
    ctx.step_count++;

    // For debugging, prove we're hitting GPU path:
    if (ctx.step_count == 1 || (ctx.step_count % 100 == 0)) {
        std::cout << "[Metal Backend] step " << ctx.step_count << " (stub)\n";
    }
}

// -----------------------------------------------------------
// metal_compute_backend_energy
// -----------------------------------------------------------
//
// For now: pull data from ctx.particles on CPU side.
// Later we can read GPU buffer back or compute energy on GPU.
//
float metal_compute_backend_energy(const BackendContext& ctx) {
    // temporary CPU-side energy calc (reuse existing compute_total_energy)
    return compute_total_energy(ctx.particles);
}

// -----------------------------------------------------------
// metal_shutdown_backend
// -----------------------------------------------------------
void metal_shutdown_backend(BackendContext& ctx) {
    MetalState* state = reinterpret_cast<MetalState*>(ctx.backend_data);
    if (state) {
        // ARC (Automatic Reference Counting) will clean up id<...> objects
        // when 'state' is deleted, because we're in Objective-C++ land
        delete state;
        ctx.backend_data = nullptr;
    }

    std::cout << "[Metal Backend] shutdown complete.\n";
}
