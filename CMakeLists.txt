cmake_minimum_required(VERSION 3.16)
project(BedrockNativeLib VERSION 1.0.0 LANGUAGES CXX C)

# Configurações do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Opções de build
option(BUILD_SHARED_LIBS "Build shared library" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build tests" OFF)

# ==================== BIBLIOTECA PRINCIPAL ====================

add_library(bedrock_lib SHARED
    bedrock_lib.cpp
    bedrock_lib.hpp
)

# Nome da biblioteca
set_target_properties(bedrock_lib PROPERTIES
    OUTPUT_NAME "bedrock_lib"
    PREFIX "lib"
    # Linux/Android: .so
    # Windows: .dll
    # macOS: .dylib
)

# Definições de compilação
target_compile_definitions(bedrock_lib PRIVATE
    BEDROCKLIB_EXPORTS
)

# Flags de compilação
target_compile_options(bedrock_lib PRIVATE
    # GCC/Clang
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:
        -Wall -Wextra -Wpedantic
        -fPIC
        -fvisibility=hidden
        -O3
    >
    # MSVC
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4 /O2
    >
)

# Link flags
target_link_options(bedrock_lib PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:
        -shared
        -Wl,-soname,libbedrock_lib.so.1
    >
)

# Includes
target_include_directories(bedrock_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# ==================== INSTALAÇÃO ====================

install(TARGETS bedrock_lib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES bedrock_lib.hpp
    DESTINATION include
)

# ==================== EXEMPLOS ====================

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# ==================== TESTES ====================

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ==================== PACOTE ====================

# Gerar configuração para find_package
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/BedrockNativeLibConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
