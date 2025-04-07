# CS-425: MPI Programming Assignment

This repository contains solutions to three MPI (Message Passing Interface) problems that demonstrate fundamental and advanced parallel programming concepts using MPI.

## Overview

The problems tackled in this assignment are:

1. **Hello World with MPI**  
2. **Sum of Array Elements**  
3. **Matrix Multiplication**  

Each problem is implemented in C/C++ using MPI, with a focus on parallelization, performance measurement, and correctness validation.

---

## Pithagoras Cluster

All MPI programs were developed and executed on a custom-built Raspberry Pi cluster named Pithagoras, consisting of 8 Raspberry Pi 4 Model B boards. Each Pi is equipped with a quad-core ARM Cortex-A72 CPU and 2 GB of RAM.

This cluster provides a cost-effective and educational platform for parallel computing and distributed systems experiments. It was used to benchmark wall time, observe speedups, and validate correctness across different levels of parallelism.

---

## Prerequisites

- C or C++ compiler (e.g., `gcc`, `g++`)
- MPI library (e.g., [OpenMPI](https://www.open-mpi.org/), [MPICH](https://www.mpich.org/))
- Linux or macOS (or WSL on Windows)

Install OpenMPI on Ubuntu:
```bash
sudo apt update
sudo apt install openmpi-bin libopenmpi-dev
