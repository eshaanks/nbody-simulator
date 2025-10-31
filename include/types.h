#pragma once
#include <vector>
#include <string>

// ===========================================================
// types.h — Core data structures and simulation constants
// ===========================================================

// Gravitational constant and default softening
constexpr float G_CONST     = 1.0f;     // normalized units
constexpr float EPSILON     = 1e-3f;    // softening term
constexpr float EPS2        = EPSILON * EPSILON;  // precomputed ε²

// -----------------------------------------------------------
// Particle structure (AoS layout for CPU version)
// -----------------------------------------------------------
struct Particle {
    float x, y, z;      // Position
    float vx, vy, vz;   // Velocity
    float ax, ay, az;   // Acceleration
    float mass;         // Mass
};

// -----------------------------------------------------------
// Simulation parameters — used by all backends
// -----------------------------------------------------------
struct SimParams {
    int    N = 1000;         // Number of particles
    float  dt = 1e-3f;       // Time step
    float  total_time = 1.0; // Simulation length
    int    output_interval = 100; // steps between logs
    std::string backend = "cpu";  // backend selector
};

// Convenience alias
using ParticleArray = std::vector<Particle>;

