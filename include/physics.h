#pragma once
#include "types.h"
#include <cmath>

// ===========================================================
// physics.h — Core physics operations (backend-agnostic)
// ===========================================================

// Compute accelerations for all particles
// Implemented in physics.cpp (CPU baseline)
void compute_accelerations(ParticleArray &particles);

// Integrate one leapfrog step
// Updates velocities and positions using current accelerations
void integrate_leapfrog(ParticleArray &particles, float dt);

// Compute kinetic + potential energy (for diagnostics)
float compute_total_energy(const ParticleArray &particles);
