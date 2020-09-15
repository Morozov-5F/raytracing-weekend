cmake_minimum_required(VERSION 3.17)
project(ray_tracing_one_week C)

set(CMAKE_C_STANDARD 11)

add_executable(ray_tracing_one_week rt_hittable.c rt_sphere.c rt_hittable_list.c main.c rt_camera.c rt_colour.c)
target_compile_options(ray_tracing_one_week PUBLIC -Wall -Wextra -Winline)
target_link_libraries(ray_tracing_one_week m)