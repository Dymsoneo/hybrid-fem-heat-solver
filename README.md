# Hybrid FEM Heat Solver

Finite Element Method (FEM) solver for transient heat conduction with integration of experimental temperature measurements.

The project is developed as part of an engineering thesis focused on modelling and monitoring the heating process of cylindrical metal samples before extrusion.

The system combines numerical simulation with real measurement data acquired using an Arduino-based temperature sensing system.

---

# Project Overview

The main objective of the project is to develop a hybrid system capable of:

- simulating transient heat conduction using the Finite Element Method
- incorporating experimental temperature measurements into the simulation
- estimating temperature distribution inside a heated cylindrical sample
- operating in two modes:
  - **Accurate mode** – high fidelity simulation
  - **Realtime mode** – optimized simulation capable of operating close to real-time

The solver is validated using both numerical benchmarks and experimental measurements from a laboratory furnace.

---

# Features

- transient heat conduction solver
- temperature dependent material properties `k(T)` and `c(T)`
- nonlinear solver using **Picard iteration**
- convection and contact boundary conditions
- axisymmetric FEM formulation
- integration with **Arduino temperature measurements**
- two simulation modes:
  - Accurate (offline)
  - Realtime (optimized)

---

# Technologies

- **C++**
- Finite Element Method
- Numerical simulation
- Arduino-based data acquisition

---

# Repository Structure

```text
hybrid-fem-heat-solver/
│
├── docs/ → theory notes and project documentation
├── data/ → meshes, material properties and measurement data
├── src/ → solver source code
├── arduino/ → Arduino data acquisition code
├── validation/ → solver verification cases
├── results/ → simulation results and plots
```

# Experimental Validation

The solver is validated using measurements from a laboratory furnace:

- cylindrical metal sample
- thermocouple placed in the center
- environmental temperature measured using Arduino sensors

Simulation results are compared with:

- experimental measurements
- numerical results obtained using SolidWorks Simulation

---

# Build Instructions

The project is intended to be built using **Visual Studio**.

Steps:

1. Clone repository
2. Open project in Visual Studio
3. Build solution
4. Run the solver

---

## Project Status

Current milestone:
Mesh structures and FEM element implementation