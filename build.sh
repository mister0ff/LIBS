#!/bin/bash

# Script de build para Bedrock Native Library
# Suporta: Linux, Android, Windows (cross-compile)

set -e

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Diretórios
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"
INSTALL_DIR="${SCRIPT_DIR}/install"

# Plataforma padrão
PLATFORM="${1:-linux}"
BUILD_TYPE="${2:-Release}"

echo -e "${GREEN}=== Bedrock Native Library Build Script ===${NC}"
echo "Plataforma: ${PLATFORM}"
echo "Build Type: ${BUILD_TYPE}"
echo ""

# Limpar build anterior
if [ "$3" == "clean" ]; then
    echo -e "${YELLOW}Limpando build anterior...${NC}"
    rm -rf "${BUILD_DIR}"
    rm -rf "${INSTALL_DIR}"
fi

mkdir -p "${BUILD_DIR}"
mkdir -p "${INSTALL_DIR}"

cd "${BUILD_DIR}"

# Configurar CMake baseado na plataforma
case "${PLATFORM}" in
    linux)
        echo -e "${YELLOW}Configurando para Linux...${NC}"
        cmake .. \
            -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
            -DCMAKE_INSTALL_PREFIX="${INSTALL_DIR}" \
            -DBUILD_SHARED_LIBS=ON \
            -DBUILD_EXAMPLES=ON
        ;;

    android)
        echo -e "${YELLOW}Configurando para Android...${NC}"

        # Verificar NDK
        if [ -z "$ANDROID_NDK" ]; then
            echo -e "${RED}Erro: ANDROID_NDK não definido!${NC}"
            echo "Defina a variável de ambiente ANDROID_NDK apontando para o Android NDK"
            exit 1
        fi

        # Android ABI
        ANDROID_ABI="${ANDROID_ABI:-arm64-v8a}"
        ANDROID_PLATFORM="${ANDROID_PLATFORM:-android-24}"

        cmake .. \
            -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
            -DCMAKE_INSTALL_PREFIX="${INSTALL_DIR}/${ANDROID_ABI}" \
            -DBUILD_SHARED_LIBS=ON \
            -DBUILD_EXAMPLES=OFF \
            -DCMAKE_SYSTEM_NAME=Android \
            -DCMAKE_ANDROID_NDK="${ANDROID_NDK}" \
            -DCMAKE_ANDROID_ARCH_ABI="${ANDROID_ABI}" \
            -DCMAKE_ANDROID_PLATFORM="${ANDROID_PLATFORM}" \
            -DCMAKE_ANDROID_STL_TYPE=c++_shared
        ;;

    windows)
        echo -e "${YELLOW}Configurando para Windows (cross-compile)...${NC}"
        cmake .. \
            -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
            -DCMAKE_INSTALL_PREFIX="${INSTALL_DIR}" \
            -DBUILD_SHARED_LIBS=ON \
            -DBUILD_EXAMPLES=ON \
            -DCMAKE_TOOLCHAIN_FILE="${SCRIPT_DIR}/cmake/mingw-toolchain.cmake"
        ;;

    *)
        echo -e "${RED}Plataforma não suportada: ${PLATFORM}${NC}"
        echo "Plataformas suportadas: linux, android, windows"
        exit 1
        ;;
esac

# Build
echo -e "${YELLOW}Compilando...${NC}"
cmake --build . --config "${BUILD_TYPE}" -j$(nproc)

# Instalar
echo -e "${YELLOW}Instalando...${NC}"
cmake --install . --config "${BUILD_TYPE}"

# Verificar resultado
if [ $? -eq 0 ]; then
    echo ""
    echo -e "${GREEN}=== Build concluído com sucesso! ===${NC}"
    echo ""
    echo "Arquivos gerados:"
    find "${INSTALL_DIR}" -type f | while read file; do
        echo "  - ${file}"
    done
    echo ""
    echo "Biblioteca: ${INSTALL_DIR}/lib/libbedrock_lib.so"
    echo "Headers: ${INSTALL_DIR}/include/bedrock_lib.hpp"
else
    echo -e "${RED}=== Build falhou! ===${NC}"
    exit 1
fi

