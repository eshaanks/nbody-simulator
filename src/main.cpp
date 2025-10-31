#include "backend.h"
#include "types.h"
#include <iostream>
#include <chrono>
#include <cstring>

// ===========================================================
// main.cpp — Entry point for N-Body Simulation (CPU baseline)
// ===========================================================

void print_usage() {
    std::cout << "Usage: ./nbody [--n N] [--dt DT] [--steps STEPS]\n";
}

int main(int argc, char **argv) {
    SimParams params;

    // -------------------------------------------------------
    // Parse simple CLI arguments
    // -------------------------------------------------------
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--n") == 0 && i + 1 < argc) {
            params.N = std::stoi(argv[++i]);
        } else if (strcmp(argv[i], "--dt") == 0 && i + 1 < argc) {
            params.dt = std::stof(argv[++i]);
        } else if (strcmp(argv[i], "--steps") == 0 && i + 1 < argc) {
            params.total_time = std::stof(argv[++i]);
        } else {
            print_usage();
            return 1;
        }
    }

    std::cout << "=== N-Body Simulation (CPU Baseline) ===\n";
    std::cout << "Particles : " << params.N << "\n";
    std::cout << "Time step : " << params.dt << "\n";
    std::cout << "Steps     : " << params.total_time << "\n\n";

    BackendContext ctx;

    // -------------------------------------------------------
    // Initialize backend
    // -------------------------------------------------------
    if (!init_backend(ctx, params)) {
        std::cerr << "Failed to initialize backend.\n";
        return 1;
    }

    // -------------------------------------------------------
    // Simulation loop
    // -------------------------------------------------------
    const int total_steps = static_cast<int>(params.total_time);
    auto start = std::chrono::high_resolution_clock::now();

    for (int step = 0; step < total_steps; ++step) {
        step_backend(ctx);

        if (step % params.output_interval == 0) {
            float E = compute_backend_energy(ctx);
            std::cout << "Step " << step
                      << " | Energy = " << E << "\n";
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    double elapsed =
        std::chrono::duration<double>(end - start).count();

    std::cout << "\nSimulation completed in "
              << elapsed << " seconds.\n";

    shutdown_backend(ctx);
    return 0;
}
