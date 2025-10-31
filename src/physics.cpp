#include "physics.h"
#include <cmath>
#include <iostream>

// ===========================================================
// physics.cpp — CPU implementation of gravitational physics
// ===========================================================

// -----------------------------------------------------------
// Compute accelerations for all particles (O(N^2))
// -----------------------------------------------------------
void compute_accelerations(ParticleArray &p) {
    const size_t N = p.size();

    // Reset accelerations
    for (auto &pi : p) {
        pi.ax = pi.ay = pi.az = 0.0f;
    }

    // Brute-force all-pairs interaction
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            float dx = p[j].x - p[i].x;
            float dy = p[j].y - p[i].y;
            float dz = p[j].z - p[i].z;
            float distSqr = dx * dx + dy * dy + dz * dz + EPS2;
            float invDist = 1.0f / std::sqrt(distSqr);
            float invDist3 = invDist * invDist * invDist;

            float force = G_CONST * p[i].mass * p[j].mass * invDist3;

            // Apply equal and opposite accelerations
            p[i].ax += force * dx / p[i].mass;
            p[i].ay += force * dy / p[i].mass;
            p[i].az += force * dz / p[i].mass;

            p[j].ax -= force * dx / p[j].mass;
            p[j].ay -= force * dy / p[j].mass;
            p[j].az -= force * dz / p[j].mass;
        }
    }
}

// -----------------------------------------------------------
// Leapfrog (Velocity Verlet) Integrator
// -----------------------------------------------------------
void integrate_leapfrog(ParticleArray &p, float dt) {
    // Half-step velocity update
    for (auto &pi : p) {
        pi.vx += 0.5f * pi.ax * dt;
        pi.vy += 0.5f * pi.ay * dt;
        pi.vz += 0.5f * pi.az * dt;
    }

    // Full position update
    for (auto &pi : p) {
        pi.x += pi.vx * dt;
        pi.y += pi.vy * dt;
        pi.z += pi.vz * dt;
    }

    // Recompute accelerations after moving
    compute_accelerations(p);

    // Second half velocity update
    for (auto &pi : p) {
        pi.vx += 0.5f * pi.ax * dt;
        pi.vy += 0.5f * pi.ay * dt;
        pi.vz += 0.5f * pi.az * dt;
    }
}

// -----------------------------------------------------------
// Energy diagnostics
// -----------------------------------------------------------
float compute_total_energy(const ParticleArray &p) {
    float kinetic = 0.0f;
    float potential = 0.0f;
    const size_t N = p.size();

    // Kinetic: ½ m v²
    for (const auto &pi : p) {
        kinetic += 0.5f * pi.mass * (pi.vx*pi.vx + pi.vy*pi.vy + pi.vz*pi.vz);
    }

    // Potential: -G m_i m_j / r_ij
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            float dx = p[j].x - p[i].x;
            float dy = p[j].y - p[i].y;
            float dz = p[j].z - p[i].z;
            float dist = std::sqrt(dx*dx + dy*dy + dz*dz + EPS2);
            potential -= G_CONST * p[i].mass * p[j].mass / dist;
        }
    }

    return kinetic + potential;
}
