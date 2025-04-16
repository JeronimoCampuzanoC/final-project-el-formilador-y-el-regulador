cmake_minimum_required(VERSION 3.16)
project(final-project-el-formilador-y-el-regulador VERSION 1.0 LANGUAGES CXX)

# Estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Incluye headers
include_directories(include)

# Archivos fuente
set(SOURCES
    src/main.cpp
    src/Reader.cpp
    src/Grammar.cpp
)

# Ejecutable
add_executable(${PROJECT_NAME} ${SOURCES})
