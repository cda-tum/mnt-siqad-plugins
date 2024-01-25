# *Munich Nanotech Toolkit (MNT)* Plugins for SiQAD

[![Ubuntu CI](https://img.shields.io/github/actions/workflow/status/cda-tum/mnt-siqad-plugins/ubuntu.yml?label=Ubuntu&logo=ubuntu&style=flat-square)](https://github.com/cda-tum/mnt-siqad-plugins/action/workflows/ubuntu.yml)
[![macOS CI](https://img.shields.io/github/actions/workflow/status/cda-tum/mnt-siqad-plugins/macos.yml?label=macOS&logo=apple&style=flat-square)](https://github.com/cda-tum/mnt-siqad-plugins/action/workflows/macos.yml)
[![Windows CI](https://img.shields.io/github/actions/workflow/status/cda-tum/mnt-siqad-plugins/windows.yml?label=Windows&logo=windows&style=flat-square)](https://github.com/cda-tum/mnt-siqad-plugins/action/workflows/windows.yml)
[![IEEEXplore](https://img.shields.io/static/v1?label=IEEEXplore&message=QuickSim&color=informational&style=flat-square)](https://ieeexplore.ieee.org/document/10231266)
[![arXiv](https://img.shields.io/static/v1?label=arXiv&message=QuickExact&color=informational&style=flat-square)](https://arxiv.org/abs/2308.04487)

<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="docs/_static/mnt_light.svg" width="60%">
    <img src="docs/_static/mnt_dark.svg" width="60%">
  </picture>
</p>

This provides two Physical Simulators for *Silicon Dangling Bond (SiDB)* Logic: [
*QuickExact*](https://www.cda.cit.tum.de/files/eda/2024_aspdac_efficient_exact_simulation.pdf)
and [*QuickSim*](https://www.cda.cit.tum.de/files/eda/2023_ieeenano_quicksim_physical_simulation.pdf).

#### General information:

Both are implemented on top of the [*fiction*](https://github.com/cda-tum/fiction) framework. This project makes the
simulators available inside the [SiQAD](https://github.com/siqad/siqad) GUI.

#### *QuickExact*

*QuickExact* is an exact simulator,
i.e., it determines __all__ physically valid charge configurations of a given layout with __100 % accuracy__.

- The parameter `simulation base number autodetection` determines whether the simulation should automatically assess and
  set the required base number before starting. When set to `0` (off), this feature is disabled. However, when set to
  `1` (on), the simulation evaluates the possibility of positively charged SiDBs occurring based on the provided
  physical
  parameters or the proximity of SiDBs. If there's a potential for positively charged SiDBs, the base number is
  dynamically set to 3. Otherwise, it is set to 2. This ensures that the simulation consistently delivers correct
  results.

#### *QuickSim*

*QuickSim* is an approximate 2-state simulator for determining the ground state of a given SiDB
layout.

- It enables the tuning of several simulation parameters: `iteration steps` and `alpha`. For gate layouts, there should
  be
  no need to adjust the default values. If no solution is found, however, `iteration steps` can be increased
  or `alpha` reduced to enable high-effort simulation.
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
cmake . -B build
cd build
cmake --build . -j4
```

### Using *QuickExact* and *QuickSim* in the SiQAD GUI

Make sure you have [SiQAD](https://github.com/siqad/siqad) installed and the plugins compiled.

Navigate to `<PATH-TO-SIQAD>/plugins/` and create a new folder called `quicksim` and `quickexact`.

Move the `quicksim` and `quickexact` binaries along with their respective `.physeng` files from
the `mnt-siqad-plugins/build/`
directory to the newly created `siqad/plugins/quicksim/` and `siqad/plugins/quickexact/` directories. Once completed,
restart SiQAD.

*QuickExact* and *QuickSim* are now available in the engine list.

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
