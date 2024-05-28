add_library(stm32-drivers INTERFACE)

target_compile_definitions(stm32-drivers INTERFACE 
	STM32F446xx
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32-drivers INTERFACE
    ./CMSIS/Include
    ./CMSIS/Device/ST/STM32F4xx/Include
)
