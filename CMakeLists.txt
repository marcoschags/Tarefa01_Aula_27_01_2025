# Generated CMake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Inicializa o SDK do Raspberry Pi Pico
set(PICO_BOARD pico_w CACHE STRING "Board type")
include(pico_sdk_import.cmake)

project(tarefa01 C CXX ASM)

# Inicializa o SDK do Pico
pico_sdk_init()

# Adiciona o executável
add_executable(tarefa01 tarefa01.c )

pico_set_program_name(tarefa01 "tarefa01")
pico_set_program_version(tarefa01 "0.1")

# Ativa a saída padrão via USB
pico_enable_stdio_uart(tarefa01 0)
pico_enable_stdio_usb(tarefa01 1)

# Adiciona as bibliotecas padrão e as necessárias
target_link_libraries(tarefa01
    pico_stdlib
    hardware_pio
    hardware_adc  # Adicionando a biblioteca do ADC
)

# Adiciona diretórios de inclusão
target_include_directories(tarefa01 PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Gera arquivos binários adicionais
pico_add_extra_outputs(tarefa01)
