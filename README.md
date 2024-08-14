Advent of Code C++
==================

All of my Advent of Code exercises written in C++

Setup
-----

1. Have [CMake](https://community.chocolatey.org/packages/cmake) installed
2. Have [vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started) installed
    - Make sure to add vcpkg to the PATH and set a VCPKG_ROOT environment variable
3. Clone this repository, with the `--recursive` flag to get the submodules
4. run `vcpkg install`
5. Open the new directory as a CMake project in Visual Studio
6. Add a [launch.vs.json](https://learn.microsoft.com/en-us/cpp/build/configure-cmake-debugging-sessions?view=msvc-170) file to configure debug settings
    - Set "currentDir" to "${workspaceRoot}"
    - Set "args" to desired setting
        - Arg 1: Exercise Year
        - Arg 2: Exercise Day
