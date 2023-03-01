# What you need

* git
* \>= Python 3.5
* \>= CMake 3.16
* A C++ compiler, either:
* 1. \>= MSVC 14.0, for Windows
* 2. \>= Clang 14, for Linux
  

# Building

1. Clone the repository recursively:  <br>
   >`git clone --recurse-submodule https://github.com/MatthewZelriche/Tremble` <br>
2. Execute the `build.py` script. You can specify the number of jobs and build config through this script.
3. Once the build is completed, you can find the built files in:
   >`build/bin/<build_config>/`
