# Advanced Adaptive Optical Matrix & Closed-Loop Control Engine

A high-performance C++17 simulation engine utilizing SFML for real-time telemetry and visual feedback control, paired with MATLAB-generated wavefront aberration matrices. Designed to model phase correction loops analogous to Liquid Crystal Spatial Light Modulators (LC-SLMs).

## 🚀 Key Features
- **Matrix Ingestion Pipeline:** Custom C++ CSV parser loading spatial wavefront phase grids.
- **Real-Time Heatmap Visualization:** Dynamic thermal color-mapping (Blue-Green-Red thermal gradient) rendering 20x20 phase matrices at 60 FPS using SFML.
- **Interactive Telemetry HUD:** On-screen data overlays displaying live gain multipliers and state variables.
- **Closed-Loop Feedback Controller:** Iterative damping algorithm simulating deformable mirror convergence toward a flat wavefront.

## 🛠️ Tech Stack
- **Language:** C++17
- **Modeling & Export:** MATLAB / Octave
- **Graphics & UI:** SFML (Simple and Fast Multimedia Library)
- **Build System:** GNU Make / Shell Scripting

## ⚙️ Getting Started & Compilation
Ensure you have SFML installed on your system, then compile and run the engine:

```bash
cd cpp
./build_engine
