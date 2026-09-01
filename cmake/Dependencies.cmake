include(FetchContent)

# fiction, siqadconn, and Catch2 used to be checked out as git submodules, each
# pinned to a fixed commit. FetchContent replaces that: each is still pinned to
# the exact same kind of reference (a commit or a tag), just resolved by CMake
# at configure time instead of by `git submodule update`, so plain `git clone`
# (no `--recursive`) is enough to build this project.
#
# fiction and siqadconn have no meaningful release cadence we track today (the
# submodule pins were arbitrary points on their default branch), so they stay
# pinned to a branch-head commit SHA via a `<DEP>_REV` variable, updated by
# Renovate's git-refs datasource. Catch2 does publish clean semver tags, so it
# is pinned to a released version instead via `<DEP>_VERSION`.

# fiction
set(FICTION_REV
    005d10ca3373ca292161c7013fa5764058923a5c
    CACHE STRING "fiction revision -- head of the main branch")
set(FICTION_CLI
    OFF
    CACHE BOOL "" FORCE)
set(FICTION_PROGRESS_BARS
    OFF
    CACHE BOOL "" FORCE)
set(FICTION_ALGLIB
    ON
    CACHE BOOL "" FORCE) # Enable ALGLIB by default to support ClusterComplete
FetchContent_Declare(
  fiction
  GIT_REPOSITORY https://github.com/cda-tum/fiction.git
  GIT_TAG ${FICTION_REV})
FetchContent_MakeAvailable(fiction)

# siqadconn is used as headers only (see libs/CMakeLists.txt), so its own
# CMakeLists.txt is never invoked -- populate the source tree without building
# it.
set(SIQADCONN_REV
    15b3cf5a67bb96945d9f7c4efedb951927a12bfa
    CACHE STRING "siqadconn revision -- head of the master branch")
FetchContent_Declare(
  siqadconn
  GIT_REPOSITORY https://github.com/siqad/siqadconn.git
  GIT_TAG ${SIQADCONN_REV})
FetchContent_Populate(siqadconn)

# Catch2 (test-only)
if(MNT_SIQAD_PLUGINS_TEST)
  set(CATCH2_VERSION
      3.15.3
      CACHE STRING "Catch2 version")
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v${CATCH2_VERSION})
  FetchContent_MakeAvailable(Catch2)
endif()
