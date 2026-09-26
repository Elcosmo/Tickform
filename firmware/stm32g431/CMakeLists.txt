cmake_minimum_required(VERSION 3.22)
project(tickform_gate4 LANGUAGES C ASM)
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
    message(FATAL_ERROR "This Gate 4 diagnostic requires STM32CubeG4 1.6.3")
endif()
set(HAL "${STM32_CUBE_G4_ROOT}/Drivers/STM32G4xx_HAL_Driver")
set(USB_DEVICE "${STM32_CUBE_G4_ROOT}/Middlewares/ST/STM32_USB_Device_Library")
add_executable(tickform_gate4
    USB_Device/usb_device.c
    USB_Device/usbd_conf.c
    USB_Device/usbd_desc.c
    "${HAL}/Src/stm32g4xx_hal.c"
    "${HAL}/Src/stm32g4xx_hal_cortex.c"
    "${HAL}/Src/stm32g4xx_hal_rcc.c"
    "${HAL}/Src/stm32g4xx_hal_rcc_ex.c"
    "${HAL}/Src/stm32g4xx_hal_pcd.c"
    "${HAL}/Src/stm32g4xx_hal_pcd_ex.c"
    "${HAL}/Src/stm32g4xx_ll_usb.c"
    "${USB_DEVICE}/Core/Src/usbd_core.c"
    "${USB_DEVICE}/Core/Src/usbd_ctlreq.c"
    "${USB_DEVICE}/Core/Src/usbd_ioreq.c"
    "${USB_DEVICE}/Class/CDC/Src/usbd_cdc.c"
    Core/Src/main.c
    "${DEVICE}/Source/Templates/system_stm32g4xx.c"
    "${DEVICE}/Source/Templates/gcc/startup_stm32g431xx.s")
set_target_properties(tickform_gate4 PROPERTIES SUFFIX ".elf")
target_include_directories(tickform_gate4 PRIVATE "${CMSIS}/Include" "${DEVICE}/Include"
    Core/Inc USB_Device "${HAL}/Inc" "${USB_DEVICE}/Core/Inc" "${USB_DEVICE}/Class/CDC/Inc")
target_compile_definitions(tickform_gate4 PRIVATE STM32G431xx USER_VECT_TAB_ADDRESS USE_HAL_DRIVER)
target_compile_options(tickform_gate4 PRIVATE
    -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -g3
    -ffunction-sections -fdata-sections -Wall -Wextra)
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/STM32G431CBU6_FLASH.ld")
target_link_options(tickform_gate4 PRIVATE
    -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -nostartfiles
    --specs=nano.specs --specs=nosys.specs
    "-T${LINKER_SCRIPT}" -Wl,--gc-sections
    "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/tickform_gate4.map"
    -Wl,--print-memory-usage)
set_property(TARGET tickform_gate4 APPEND PROPERTY LINK_DEPENDS "${LINKER_SCRIPT}")
add_custom_command(TARGET tickform_gate4 POST_BUILD
    COMMAND "${CMAKE_OBJCOPY}" -O ihex $<TARGET_FILE:tickform_gate4> "${CMAKE_CURRENT_BINARY_DIR}/tickform_gate4.hex"
    COMMAND "${CMAKE_OBJCOPY}" -O binary $<TARGET_FILE:tickform_gate4> "${CMAKE_CURRENT_BINARY_DIR}/tickform_gate4.bin"
    COMMAND "${CMAKE_SIZE}" $<TARGET_FILE:tickform_gate4>
    VERBATIM)
