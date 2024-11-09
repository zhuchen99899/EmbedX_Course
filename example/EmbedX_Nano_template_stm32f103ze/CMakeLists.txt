set(__PROJ_NAME__ "Template"      CACHE INTERNAL "Proj name")
set(__CHIP_TYPE__ "STM32F103ZE" CACHE INTERNAL "Chip type")
set(__TOOLCHAIN__ "ARMGCC"      CACHE INTERNAL "Toolchain")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/project/GCC/output)
set(HEX_FILE ${EXECUTABLE_OUTPUT_PATH}/${__PROJ_NAME__}.hex)
set(BIN_FILE ${EXECUTABLE_OUTPUT_PATH}/${__PROJ_NAME__}.bin)
set(MAP_FILE ${EXECUTABLE_OUTPUT_PATH}/${__PROJ_NAME__}.map)

SET(CMAKE_BUILD_TYPE "Debug")

# -----------------------------------------------------------------------------
# 指定 CMake 最低要求版本
#
cmake_minimum_required(VERSION 3.20)


# -----------------------------------------------------------------------------
# 引入交叉编译工具链配置文件
#
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/project/GCC/scripts/cmake/armgcc_cm3.cmake")


# -----------------------------------------------------------------------------
# 生成 compile_commands.json 文件 配合 VSCode clangd 插件实现精准代码跳转与提示
#
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# -----------------------------------------------------------------------------
# 指定工程名称和语言列表
#
project(${__PROJ_NAME__} LANGUAGES C CXX ASM)


# -----------------------------------------------------------------------------
# 引入工程依赖基础库和第三方组件
#
add_subdirectory(platform/mcu/stm32f103ze/bsp)
add_subdirectory(platform/mcu/stm32f103ze/libraries) # libstm32f10x_hal.a






file(GLOB app_srcs     CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/app/*.c")


add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE "${app_srcs}")


target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/app")

target_link_libraries(${PROJECT_NAME} PRIVATE "bsp" )
target_link_libraries(${PROJECT_NAME} PRIVATE "stm32f10x_hal" )








target_link_options(${PROJECT_NAME}
PRIVATE
	-Wl,-Map=${MAP_FILE}
)

#目标为 elf 文件便于调试
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${__PROJ_NAME__}.elf"
    RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)

#转为hex和bin文件方烧写
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}> ${HEX_FILE}
	COMMAND ${OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}> ${BIN_FILE}
	COMMAND ${SIZE} --format=sysv --radix=16 --total $<TARGET_FILE:${PROJECT_NAME}>
)



