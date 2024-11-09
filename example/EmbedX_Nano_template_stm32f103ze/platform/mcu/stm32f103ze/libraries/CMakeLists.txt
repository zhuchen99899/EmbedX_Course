set(LIB_NAME "stm32f10x_hal")

file(GLOB stm32f10x_hal_srcs CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/STM32F1xx_HAL_Driver/Src/*.c")



add_library(${LIB_NAME} STATIC "")




target_sources(${LIB_NAME} PUBLIC "${stm32f10x_hal_srcs}")


target_include_directories(${LIB_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/CMSIS/Include")
target_include_directories(${LIB_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/CMSIS/Device/ST/STM32F1xx/Include")
target_include_directories(${LIB_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/STM32F1xx_HAL_Driver/Inc")
target_include_directories(${LIB_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/STM32F1xx_HAL_Driver/Inc/Legacy")


target_include_directories(${LIB_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/STM32F1xx_HAL_Driver/Inc/Legacy")

target_sources(${LIB_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../core/system_stm32f1xx.c")
target_sources(${LIB_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../core/GCC/startup_stm32f103xe.s")

target_include_directories(${LIB_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../core/include")

if(__CHIP_TYPE__ STREQUAL "STM32F103ZE")
    target_compile_definitions(${LIB_NAME}
    PUBLIC
        USE_HAL_DRIVER
        STM32F103xE
    )

    target_link_options(${LIB_NAME}
    PUBLIC
        -T "${CMAKE_CURRENT_LIST_DIR}/../core/GCC/STM32F103ZETx_FLASH.ld"
    )
endif()
