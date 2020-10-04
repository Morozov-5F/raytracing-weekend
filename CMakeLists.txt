cmake_minimum_required(VERSION 3.17)

project(ray_tracing_one_week C)

set(CMAKE_C_STANDARD 11)

include(CheckLibraryExists)

CHECK_LIBRARY_EXISTS(m cos "" HAVE_LIB_M)
if (HAVE_LIB_M)
    set(EXTRA_LIBS ${EXTRA_LIBS} m)
endif()


add_executable(ray_tracing_one_week rt_hittable.c rt_sphere.c rt_hittable_list.c main.c rt_camera.c rt_colour.c rt_material.c rt_hit.c rt_material_diffuse.c rt_material_metal.c rt_material_dielectric.c)
target_compile_options(ray_tracing_one_week PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Wall -Wextra -Winline>
        $<$<CXX_COMPILER_ID:MSVC>:
        /WX /W4>)

target_link_libraries(ray_tracing_one_week ${EXTRA_LIBS})