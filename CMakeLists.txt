# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# Generated CMake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Inicializa o SDK do Raspberry Pi Pico
set(PICO_BOARD pico_w CACHE STRING "Board type")
include(pico_sdk_import.cmake)

project(TarefaAnimacao C CXX ASM)

# Inicializa o SDK do Pico
pico_sdk_init()

# Adiciona o executável
add_executable(TarefaAnimacao TarefaAnimacao.c )

pico_set_program_name(TarefaAnimacao "TarefaAnimacao")
pico_set_program_version(TarefaAnimacao "0.1")

# Gera o cabeçalho .h a partir do .pio
pico_generate_pio_header(TarefaAnimacao ${CMAKE_CURRENT_LIST_DIR}/TarefaAnimacao.pio)


# Ativa a saída padrão via USB
pico_enable_stdio_uart(TarefaAnimacao 0)
pico_enable_stdio_usb(TarefaAnimacao 1)

# Adiciona as bibliotecas padrão e as necessárias
target_link_libraries(TarefaAnimacao
    pico_stdlib
    hardware_pio
    hardware_adc  # Adicionando a biblioteca do ADC
)

# Adiciona diretórios de inclusão
target_include_directories(TarefaAnimacao PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Gera arquivos binários adicionais
pico_add_extra_outputs(TarefaAnimacao)
