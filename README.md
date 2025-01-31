# *Munich Nanotech Toolkit (MNT)* Plugins for SiQAD

[![Ubuntu CI](https://img.shields.io/github/actions/workflow/status/cda-tum/mnt-siqad-plugins/ubuntu.yml?label=Ubuntu&logo=ubuntu&style=flat-square)](https://github.com/cda-tum/mnt-siqad-plugins/action/workflows/ubuntu.yml)
[![macOS CI](https://img.shields.io/github/actions/workflow/status/cda-tum/mnt-siqad-plugins/macos.yml?label=macOS&logo=apple&style=flat-square)](https://github.com/cda-tum/mnt-siqad-plugins/action/workflows/macos.yml)
[![Windows CI](https://img.shields.io/github/actions/workflow/status/cda-tum/mnt-siqad-plugins/windows.yml?label=Windows&logo=data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiPz4KPHN2ZyB2ZXJzaW9uPSIxLjEiIHZpZXdCb3g9IjAgMCAyMyAyMyIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KIDxwYXRoIGQ9Ik0xIDFoMTB2MTBIMXoiIGZpbGw9IiNmMzUzMjUiLz4KIDxwYXRoIGQ9Ik0xMiAxaDEwdjEwSDEyeiIgZmlsbD0iIzgxYmMwNiIvPgogPHBhdGggZD0iTTEgMTJoMTB2MTBIMXoiIGZpbGw9IiMwNWE2ZjAiLz4KIDxwYXRoIGQ9Ik0xMiAxMmgxMHYxMEgxMnoiIGZpbGw9IiNmZmJhMDgiLz4KPC9zdmc+Cg==&style=flat-square)](https://github.com/cda-tum/mnt-siqad-plugins/action/workflows/windows.yml)
[![IEEEXplore](https://img.shields.io/static/v1?label=IEEEXplore&message=QuickSim&color=informational&style=flat-square)](https://ieeexplore.ieee.org/document/10231266)
[![ACM](https://img.shields.io/static/v1?label=ACM&message=QuickExact&color=informational&style=flat-square)](https://dl.acm.org/doi/10.1109/ASP-DAC58780.2024.10473946)

<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="docs/_static/mnt_light.svg" width="60%">
    <img src="docs/_static/mnt_dark.svg" width="60%">
  </picture>
</p>

This provides three Physical Simulators for *Silicon Dangling Bond (SiDB)* Logic: [*ClusterComplete*](https://www.math.ru.nl/~bosma/Students/WillemLambooyMSc.pdf), [
*QuickExact*](https://www.cda.cit.tum.de/files/eda/2024_aspdac_efficient_exact_simulation.pdf)
and [*QuickSim*](https://www.cda.cit.tum.de/files/eda/2023_ieeenano_quicksim_physical_simulation.pdf).

#### General information:

Each of these is implemented on top of the [*fiction*](https://github.com/cda-tum/fiction) framework. This project makes the
simulators available inside the [SiQAD](https://github.com/siqad/siqad) GUI.

#### *ClusterComplete*

*ClusterComplete* is an exact simulator,
i.e., it determines __all__ physically valid charge configurations of a given layout with __100% accuracy__.

- The pre-simulation step offers configurability over some of its filtering endeavors: `Witness partitioning limit` and
  `Overlapping witness limit` (always smaller or equal than the former) control limits before an NP-complete sub-problem 
  is being solved. The default values ensure that the factorial scaling in the latter parameter is not noticeable in the 
  total runtime. For large simulation problems, it may be beneficial to raise these parameters---their effect can be 
  quickly observed by setting `Show pre-simulation statistics` to `1` (on). This reveals a number that is proportional 
  to the total runtime, and might be lowered for the given simulation problem when the aforementioned limits are raised. 
- The `Instance count` parameter gives control over the number of threads used for the simulation.
  When set to `-1`, *ClusterComplete* maximizes computational resources by utilizing all available threads, optimizing
  simulation efficiency.

#### *QuickExact*

*QuickExact* is an exact simulator,
i.e., it determines __all__ physically valid charge configurations of a given layout with __100% accuracy__.

- The parameter `Simulation base number autodetection` determines whether the simulation should automatically assess and
  set the required base number before starting. When set to `0` (off), this feature is disabled. However, when set to
  `1` (on), the simulation evaluates the possibility of positively charged SiDBs occurring based on the provided
  physical
  parameters or the proximity of SiDBs. If there's a potential for positively charged SiDBs, the base number is
  dynamically set to 3. Otherwise, it is set to 2. This ensures that the simulation consistently delivers correct
  results.

#### *QuickSim*

*QuickSim* is an approximate 2-state simulator for determining the ground state of a given SiDB
layout.

- It enables the tuning of several simulation parameters: `Iteration steps` and `Alpha`. For gate layouts, there should
  be
  no need to adjust the default values. If no solution is found, however, `Iteration steps` can be increased
  or `Alpha` reduced to enable high-effort simulation.
- The `Instance count` parameter gives control over the number of threads used for the simulation.
  When set to `-1`, *QuickSim* maximizes computational resources by utilizing all available threads, optimizing
  simulation efficiency.

## Quick Start

All you need is a `git` client, `cmake`, the [`Boost`](https://www.boost.org/) libraries, and a modern C++ compiler.

### Building the Plugins

> Clone the repository and its submodules:

```bash
git clone --recursive https://github.com/cda-tum/mnt-siqad-plugins.git
```

> Inside the newly cloned `mnt-siqad-plugins` folder, trigger the build process:

```bash
cmake . -B build -DFICTION_Z3=OFF -DFICTION_BENCHMARK=OFF -DFICTION_PROGRESS_BARS=OFF -DMOCKTURTLE_EXAMPLES=OFF
cd build
cmake --build . -j4
```

> **NB:** When building *ClusterComplete*, it is required to additionally pass `-DFICTION_ALGLIB=ON`. Furthermore, 
  *ClusterComplete* can be made significantly faster by passing `-DFICTION_ENABLE_JEMALLOC=ON`, although this CMake 
  option should not be passed when building *QuickSim*. See more information 
  [here](https://fiction.readthedocs.io/en/latest/getting_started.html#usage-of-jemalloc).

### Using *ClusterComplete*, *QuickExact* and *QuickSim* in the SiQAD GUI

Make sure you have [SiQAD](https://github.com/siqad/siqad) installed and the plugins compiled.

Navigate to `<PATH-TO-SIQAD>/plugins/` and create new folders named `clustercomplete`, `quicksim` and `quickexact`.

Move the `clustercomplete`, `quicksim` and `quickexact` binaries along with their respective `.physeng` files from
the `mnt-siqad-plugins/build/` directory to the newly created `siqad/plugins/quicksim/` and `siqad/plugins/quickexact/` 
directories. Once completed, restart SiQAD.

*ClusterComplete*, *QuickExact* and *QuickSim* are now available in the engine list.

## References

If you use *QuickExact* or *QuickSim* in your work, we would appreciate it if you cite the following publication:

```bibtex
@inproceedings{drewniok2023quicksim,
  author={Drewniok, Jan and Walter, Marcel and Hang Ng, Samuel Sze and Walus, Konrad and Wille, Robert},
  title={{\emph{QuickSim}: Efficient \emph{and} Accurate Physical Simulation of Silicon Dangling Bond Logic}},
  year={2023},
  pages={817-822},
  doi={10.1109/NANO58406.2023.10231266}}

@inproceedings{drewniok2024quickexact,
  author={Drewniok, Jan and Walter, Marcel and Wille, Robert},
  title={{The Need for Speed: Efficient Exact Simulation of Silicon Dangling Bond Logic}},
  booktitle={Asia and South Pacific Design Automation Conference (ASP-DAC)},
  year={2024}}
```
