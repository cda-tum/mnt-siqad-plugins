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

# siqadconn is used as headers only (see libs/CMakeLists.txt). Its own
# CMakeLists.txt does real work we don't want here (a separate project(),
# forcing CXX_STANDARD 11 on its own library target, requiring Boost/SWIG), so
# it must never be add_subdirectory()'d. Pointing SOURCE_SUBDIR at a path that
# doesn't exist in the populated source tree is CMake's documented way to make
# FetchContent_MakeAvailable() skip that add_subdirectory() step while still
# populating the source -- this avoids FetchContent_Populate()'s single-name
# form, which CMake 3.30's CMP0169 policy makes a hard error under its NEW
# (default) behavior.
set(SIQADCONN_REV
    15b3cf5a67bb96945d9f7c4efedb951927a12bfa
    CACHE STRING "siqadconn revision -- head of the master branch")
FetchContent_Declare(
  siqadconn
  GIT_REPOSITORY https://github.com/siqad/siqadconn.git
  GIT_TAG ${SIQADCONN_REV}
  SOURCE_SUBDIR do-not-build)
FetchContent_MakeAvailable(siqadconn)

# Catch2 (test-only). Pinned to the release commit's own SHA rather than its
# `vX.Y.Z` tag name: an annotated tag is a separate, mutable ref that could in
# principle be repointed at different content without this file changing,
# whereas a commit SHA is self-verifying. The version comment is Renovate
# metadata only (kept in sync with the digest on every bump), matching the same
# sha-plus-version-comment idiom already used to pin GitHub Actions in this
# repo's workflows.
if(MNT_SIQAD_PLUGINS_TEST)
  set(CATCH2_REV
      8b08d4d79514f45f7e4ce2a607ac9c94e920d1bb # v3.15.3
      CACHE STRING "Catch2 revision")
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG ${CATCH2_REV})
  FetchContent_MakeAvailable(Catch2)
endif()
