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

# Physical Model

The solver models transient heat conduction in an axisymmetric domain using the equation:

ρc ∂T/∂t = ∇ · (k ∇T)

where:

- T – temperature
- ρ – material density
- c – specific heat
- k – thermal conductivity

Boundary conditions include convection heat transfer with ambient temperature.

# Features

- transient heat conduction solver
- axisymmetric FEM formulation
- structured mesh generator
- temperature dependent material properties `k(T)` and `c(T)`
- nonlinear solver using **Picard iteration**
- convection boundary conditions
- integration with **Arduino temperature measurements**
- two simulation modes:
  - Accurate (offline)
  - Realtime (optimized)
- validated against SolidWorks Simulation

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

# Current Solver Architecture

The solver currently consists of the following core modules:

- **Mesh**
  - Node
  - Element
  - Mesh container
  - Structured mesh generator

- **FEM Core**
  - Universal element definition
  - Gauss integration scheme
  - Jacobian transformation
  - Local element matrices

- **Boundary Conditions**
  - Convection boundary matrix (Hbc)
  - Load vector (P)

- **Assembly**
  - Global matrix assembly for H, C and boundary contributions

The solver currently supports transient heat conduction and has been validated against a commercial FEM tool.

Further development will include:
- nonlinear material models (k(T), c(T))
- realtime simulation mode
- integration with experimental data acquisition

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
- nonlinear material model: `k(T)`, `c(T)`
- implementation of Picard iteration
- development of realtime simulation mode
- integration with Arduino measurement system