#include "types.h"
#include "physics.h"
#include "backend_dispatch.h"
#include <iostream>
#include <chrono>
#include <cstring>

// ===========================================================
// main.cpp — N-Body Simulation Entry Point (Refactored)
// ===========================================================

int main(int argc, char** argv) {
    // -------------------------------
    // Parse CLI arguments
    // -------------------------------
    int N = 500;
    float dt = 0.001f;
    int steps = 1000;
    std::string backend_name = "cpu";

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--n") == 0 && i + 1 < argc)
            N = std::stoi(argv[++i]);
        else if (strcmp(argv[i], "--dt") == 0 && i + 1 < argc)
            dt = std::stof(argv[++i]);
        else if (strcmp(argv[i], "--steps") == 0 && i + 1 < argc)
            steps = std::stoi(argv[++i]);
        else if (strcmp(argv[i], "--backend") == 0 && i + 1 < argc)
            backend_name = argv[++i];
    }

    // -------------------------------
    // Configure simulation parameters
    // -------------------------------
    SimParams params;
    params.N = N;
    params.dt = dt;

    // -------------------------------
    // Select backend
    // -------------------------------
    if (!select_backend(backend_name)) {
        std::cerr << "Warning: Unknown backend, defaulting to CPU.\n";
    }
    const auto& api = get_active_backend();

    // -------------------------------
    // Initialize simulation
    // -------------------------------
    BackendContext ctx;
    api.init(ctx, params);

    auto start = std::chrono::high_resolution_clock::now();

    // -------------------------------
    // Simulation loop
    // -------------------------------
    for (int step = 0; step < steps; ++step) {
        api.step(ctx);
        if (step % 100 == 0) {
            float E = api.energy(ctx);
            std::cout << "Step " << step << " | Energy = " << E << "\n";
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "\nSimulation completed in " << duration.count() << " seconds.\n";

    api.shutdown(ctx);
    return 0;
}
