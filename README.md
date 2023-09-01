# QuickExact Plugin for SiQAD

[![Ubuntu CI](https://img.shields.io/github/actions/workflow/status/cda-tum/mnt-siqad-plugin/ubuntu.yml?label=Ubuntu&logo=ubuntu&style=flat-square)](https://github.com/cda-tum/mnt-siqad-plugins/actions/workflows/ubuntu.yml)
[![macOS CI](https://img.shields.io/github/actions/workflow/status/cda-tum/mnt-siqad-plugin/macos.yml?label=macOS&logo=apple&style=flat-square)](https://github.com/cda-tum/mnt-siqad-plugins/actions/workflows/macos.yml)
[![Windows CI](https://img.shields.io/github/actions/workflow/status/cda-tum/mnt-siqad-plugin/windows.yml?label=Windows&logo=windows&style=flat-square)](https://github.com/cda-tum/mnt-siqad-plugins/actions/workflows/windows.yml)
[![arXiv](https://img.shields.io/static/v1?label=arXiv&message=2303.03422&color=informational&style=flat-square)](http://arxiv.org/abs/2303.03422)

*QuickExact* is an **efficient exact** physical simulation algorithm for determining the charge distribution of
atomic *Silicon Dangling Bond (SiDB)* assemblies.

QuickExact is implemented on top of the [*fiction*](https://github.com/cda-tum/fiction/) framework. This plugin makes
the
algorithm available inside the [SiQAD](https://github.com/siqad/siqad) GUI.

#### General information:

- QuickExact returns physically valid charge configurations only.

## Quick Start

All you need is a `git` client, `cmake`, the [`Boost`](https://www.boost.org/) libraries, and a modern C++ compiler.

### Building the Plugin

> Clone the repository and its submodules:

```bash
git clone --recursive https://github.com/cda-tum/mnt-siqad-plugins.git
```

> Inside the newly cloned `mnt-siqad-plugin` folder, trigger the build process:

```bash
cmake . -B build
cd build
cmake --build . -j4
```

### Using QuickExact in the SiQAD GUI

Make sure you have [SiQAD](https://github.com/siqad/siqad) installed and the QuickExact plugin compiled.

Navigate to `<PATH-TO-SIQAD>/plugins/` and create a new folder called `quickexact`.

Move the `quickexact` binary and the `quickexact.physeng` file from `mnt-siqad-plugin/build/` to the newly
created`siqad/plugins/quickexact/` and restart SiQAD.

QuickExact is now available in the engine list.
