
project(mlx90640)

add_library(mlx90640 MLX90640_API.cpp MLX90640_API.h MLX90640_I2C_Driver.cpp MLX90640_I2C_Driver.h )



                      
# idf_component_register(SRCS "ethernet_example_main.c"
#                        INCLUDE_DIRS ".")