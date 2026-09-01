# _Munich Nanotech Toolkit (MNT)_ Plugins for SiQAD

[![Ubuntu CI](https://img.shields.io/github/check-runs/cda-tum/mnt-siqad-plugins/main?nameFilter=%F0%9F%90%A7%20Ubuntu&label=Ubuntu&logo=ubuntu&style=flat-square)](https://github.com/cda-tum/mnt-siqad-plugins/actions/workflows/ci.yml)
[![macOS CI](https://img.shields.io/github/check-runs/cda-tum/mnt-siqad-plugins/main?nameFilter=%F0%9F%8D%8E%20macOS&label=macOS&logo=apple&style=flat-square)](https://github.com/cda-tum/mnt-siqad-plugins/actions/workflows/ci.yml)
[![Windows CI](https://img.shields.io/github/check-runs/cda-tum/mnt-siqad-plugins/main?nameFilter=%F0%9F%AA%9F%20Windows&label=Windows&logo=data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiPz4KPHN2ZyB2ZXJzaW9uPSIxLjEiIHZpZXdCb3g9IjAgMCAyMyAyMyIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KIDxwYXRoIGQ9Ik0xIDFoMTB2MTBIMXoiIGZpbGw9IiNmMzUzMjUiLz4KIDxwYXRoIGQ9Ik0xMiAxaDEwdjEwSDEyeiIgZmlsbD0iIzgxYmMwNiIvPgogPHBhdGggZD0iTTEgMTJoMTB2MTBIMXoiIGZpbGw9IiMwNWE2ZjAiLz4KIDxwYXRoIGQ9Ik0xMiAxMmgxMHYxMEgxMnoiIGZpbGw9IiNmZmJhMDgiLz4KPC9zdmc+Cg==&style=flat-square)](https://github.com/cda-tum/mnt-siqad-plugins/actions/workflows/ci.yml)
[![CodeQL](https://img.shields.io/github/check-runs/cda-tum/mnt-siqad-plugins/main?nameFilter=%F0%9F%93%9D%20CodeQL%20Analysis&label=CodeQL&logo=github&style=flat-square)](https://github.com/cda-tum/mnt-siqad-plugins/actions/workflows/ci.yml)
[![IEEEXplore](https://img.shields.io/static/v1?label=IEEEXplore&message=QuickSim&color=informational&style=flat-square)](https://ieeexplore.ieee.org/document/10231266)
[![ACM](https://img.shields.io/static/v1?label=ACM&message=QuickExact&color=informational&style=flat-square)](https://dl.acm.org/doi/10.1109/ASP-DAC58780.2024.10473946)

<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="docs/_static/mnt_light.svg" width="60%">
    <img src="docs/_static/mnt_dark.svg" width="60%">
  </picture>
</p>

This provides three Physical Simulators for _Silicon Dangling Bond (SiDB)_ Logic: [_ClusterComplete_](https://www.math.ru.nl/~bosma/Students/WillemLambooyMSc.pdf), [_QuickExact_](https://www.cda.cit.tum.de/files/eda/2024_aspdac_efficient_exact_simulation.pdf)
and [_QuickSim_](https://www.cda.cit.tum.de/files/eda/2023_ieeenano_quicksim_physical_simulation.pdf).

## General information:

Each of these is implemented on top of the [_fiction_](https://github.com/cda-tum/fiction) framework. This project makes the
simulators available inside the [SiQAD](https://github.com/siqad/siqad) GUI.

### _ClusterComplete_

_ClusterComplete_ is an exact simulator,
i.e., it determines **all** physically valid charge configurations of a given layout with **100% accuracy**.

- The pre-simulation step offers configurability over some of its filtering endeavors: `Witness partitioning limit` and
  `Overlapping witness limit` (always smaller or equal than the former) control limits before an NP-complete sub-problem
  is being solved. The default values ensure that the factorial scaling in the latter parameter is not noticeable in the
  total runtime. For large simulation problems, it may be beneficial to raise these parameters---their effect can be
  quickly observed by setting `Show pre-simulation statistics` to `1` (on). This reveals a number that is proportional
  to the total runtime, and might be lowered for the given simulation problem when the aforementioned limits are raised.
- The `Instance count` parameter gives control over the number of threads used for the simulation.
  When set to `-1`, _ClusterComplete_ maximizes computational resources by utilizing all available threads, optimizing
  simulation efficiency.

### _QuickExact_

_QuickExact_ is an exact simulator,
i.e., it determines **all** physically valid charge configurations of a given layout with **100% accuracy**.

- The parameter `Simulation base number autodetection` determines whether the simulation should automatically assess and
  set the required base number before starting. When set to `0` (off), this feature is disabled. However, when set to
  `1` (on), the simulation evaluates the possibility of positively charged SiDBs occurring based on the provided
  physical
  parameters or the proximity of SiDBs. If there's a potential for positively charged SiDBs, the base number is
  dynamically set to 3. Otherwise, it is set to 2. This ensures that the simulation consistently delivers correct
  results.

### _QuickSim_

_QuickSim_ is an approximate 2-state simulator for determining the ground state of a given SiDB
layout.

- It enables the tuning of several simulation parameters: `Iteration steps` and `Alpha`. For gate layouts, there should
  be
  no need to adjust the default values. If no solution is found, however, `Iteration steps` can be increased
  or `Alpha` reduced to enable high-effort simulation.
- The `Instance count` parameter gives control over the number of threads used for the simulation.
  When set to `-1`, _QuickSim_ maximizes computational resources by utilizing all available threads, optimizing
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
cmake . -B build
cd build
cmake --build . -j4  # replace "4" with the number of CPU cores you want to use for the build process
```

#### Building a faster _ClusterComplete_ binary

> _ClusterComplete_ can be made significantly faster by passing `-DFICTION_ENABLE_JEMALLOC=ON`, i.e.:

```bash
cmake . -B build -DFICTION_ENABLE_JEMALLOC=ON
cd build
cmake --build . -j4
```

> **NB:** This CMake option should not be passed when building _QuickSim_ as it has an adverse effect on the runtime for this plugin.
> Find more information in [the fiction jemalloc documentation](https://fiction.readthedocs.io/en/latest/getting_started.html#usage-of-jemalloc).

### Using _ClusterComplete_, _QuickExact_ and _QuickSim_ in the SiQAD GUI

Make sure you have [SiQAD](https://github.com/siqad/siqad) installed and the plugins compiled.

Navigate to `<PATH-TO-SIQAD>/plugins/` and create new folders named `clustercomplete`, `quicksim` and `quickexact`.

Move the `clustercomplete`, `quicksim` and `quickexact` binaries from the `mnt-siqad-plugins/build/plugins/` directory,
along with their respective `.physeng` and `logo-mnt.png` files from the neighboring
`mnt-siqad-plugins/build/plugins/<plugin-name>-files/` directory, to the newly created `siqad/plugins/quicksim/` and
`siqad/plugins/quickexact/` directories. Once completed, restart SiQAD.

_ClusterComplete_, _QuickExact_ and _QuickSim_ are now available in the engine list.

## References

If you use _QuickExact_ or _QuickSim_ in your work, we would appreciate it if you cite the following publication:

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
