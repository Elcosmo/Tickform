cmake_minimum_required(VERSION 3.22)
project(tickform_gate2 LANGUAGES C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
if(NOT DEFINED STM32_CUBE_G4_ROOT)
    message(FATAL_ERROR "Pass -DSTM32_CUBE_G4_ROOT=<installed STM32Cube_FW_G4_V1.6.3>")
endif()
file(TO_CMAKE_PATH "${STM32_CUBE_G4_ROOT}" STM32_CUBE_G4_ROOT)
set(CMSIS "${STM32_CUBE_G4_ROOT}/Drivers/CMSIS")
set(DEVICE "${CMSIS}/Device/ST/STM32G4xx")
file(READ "${STM32_CUBE_G4_ROOT}/package.xml" PACKAGE_INFO)
if(NOT PACKAGE_INFO MATCHES "FW.G4.1.6.3")
    message(FATAL_ERROR "This Gate 2 baseline requires STM32CubeG4 1.6.3")
endif()
add_executable(tickform_gate2
    Core/Src/main.c
    "${DEVICE}/Source/Templates/system_stm32g4xx.c"
    "${DEVICE}/Source/Templates/gcc/startup_stm32g431xx.s")
set_target_properties(tickform_gate2 PROPERTIES SUFFIX ".elf")
target_include_directories(tickform_gate2 PRIVATE "${CMSIS}/Include" "${DEVICE}/Include")
target_compile_definitions(tickform_gate2 PRIVATE STM32G431xx USER_VECT_TAB_ADDRESS)
target_compile_options(tickform_gate2 PRIVATE
    -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -g3
    -ffunction-sections -fdata-sections -Wall -Wextra)
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/STM32G431CBU6_FLASH.ld")
target_link_options(tickform_gate2 PRIVATE
    -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -nostartfiles
    --specs=nano.specs --specs=nosys.specs
    "-T${LINKER_SCRIPT}" -Wl,--gc-sections
    "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/tickform_gate2.map"
    -Wl,--print-memory-usage)
set_property(TARGET tickform_gate2 APPEND PROPERTY LINK_DEPENDS "${LINKER_SCRIPT}")
add_custom_command(TARGET tickform_gate2 POST_BUILD
    COMMAND "${CMAKE_OBJCOPY}" -O ihex $<TARGET_FILE:tickform_gate2> "${CMAKE_CURRENT_BINARY_DIR}/tickform_gate2.hex"
    COMMAND "${CMAKE_OBJCOPY}" -O binary $<TARGET_FILE:tickform_gate2> "${CMAKE_CURRENT_BINARY_DIR}/tickform_gate2.bin"
    COMMAND "${CMAKE_SIZE}" $<TARGET_FILE:tickform_gate2>
    VERBATIM)
