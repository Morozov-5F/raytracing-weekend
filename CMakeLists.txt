cmake_minimum_required(VERSION 3.17)
project(ray_tracing_one_week C)

set(CMAKE_C_STANDARD 11)

add_executable(ray_tracing_one_week rt_hittable.c rt_sphere.c rt_hittable_list.c main.c rt_camera.c rt_colour.c rt_material.c rt_hit.c rt_material_diffuse.c rt_material_metal.c rt_material_metal.h)
target_compile_options(ray_tracing_one_week PUBLIC -Wall -Wextra -Winline)
target_link_libraries(ray_tracing_one_week m)