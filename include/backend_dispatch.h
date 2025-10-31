#pragma once
#include "backend.h"
#include <string>

// ===========================================================
// backend_dispatch.h — runtime backend selection
// ===========================================================

// Available backends
enum class BackendType {
    CPU,
    METAL,
    CUDA
};

// Function pointer table for active backend
struct BackendAPI {
    bool (*init)(BackendContext&, const SimParams&);
    void (*step)(BackendContext&);
    float (*energy)(const BackendContext&);
    void (*shutdown)(BackendContext&);
};

// Returns currently active backend API
const BackendAPI& get_active_backend();

// Select backend at runtime ("cpu", "metal", "cuda")
bool select_backend(const std::string& name);
