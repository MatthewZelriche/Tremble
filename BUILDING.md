# What you need

* git
* \>= Python 3.5
* \>= CMake 3.16
* A C++ compiler, either:
* 1. \>= MSVC 14.0, for Windows
* 2. \>= Clang 14, for Linux
  

# Building

1. Clone the repository recursively:  <br>
   >'git clone --recurse-submodule https://github.com/MatthewZelriche/Tremble` <br>
   
2. If you are building for linux, an additional step is necessary. If you are building for windows, skip to step 3.
Navigate to the following file:
   >`vendor/Physx/physx/buildtools/presets/public/linux.xml` <br>
   
   and change `PX_GENERATE_STATIC_LIBRARIES` to `false`, to ensure that Physx builds dynamic libraries instead of static libraries.
3. Execute the `build.py` script. You can specify the number of jobs and build config through this script.
4. Once the build is completed, you can find the built files in:
   >`build/bin/<build_config>/`
