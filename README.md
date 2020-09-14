# raytracing-weekend
Simple path rendered written in modern C. Based on first book from https://github.com/RayTracing/raytracing.github.io

This project uses [PPM image format](https://en.wikipedia.org/wiki/Netpbm#PPM_example) to simplify image output routines. This is far from optimal but should work.

![Sample image](/images/img.jpg)

# Building
This project uses CMake to generate build system files. 

1. Create a build directory:
   ```
   ? mkdir -p build
   ```
1. Go to new directory:
   ```
   ? cd build
   ```
1. Run CMake to generate project. I recommend [Ninja](https://ninja-build.org/) as a build system, but UNIX Makefiles will work too.
   ``` bash
   # To use Ninja
   ? cmake -G Ninja ..
   ```
   or
   ``` bash
   # To use CMake (under Linux)
   ? cmake ..
   ```
1. Build the application:
   ``` bash
   # For ninja
   ? ninja build
   ```
   or 
   ``` bash
   # For makefile
   ? make
   ```
1. Launch the application and view image
   ``` bash
   ? ./ray_tracing_one_week > image.ppm
   # The following line should open file in image viewer right from the console although it may not work -- depends on the distro settings
   ? xgd-open image.ppm
   ```
