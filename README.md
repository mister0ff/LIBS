# Bedrock Native Library

Biblioteca C++ nativa para integração com Minecraft Bedrock Edition.

## 📋 Estrutura do Projeto

```
bedrock_native_lib/
├── bedrock_lib.hpp          # Header da API
├── bedrock_lib.cpp          # Implementação da biblioteca
├── CMakeLists.txt           # Configuração do CMake
├── build.sh                 # Script de build automatizado
├── examples/
│   ├── example_usage.cpp    # Exemplo de uso em C++
│   └── CMakeLists.txt       # Build dos exemplos
└── bedrock_script/
    ├── manifest.json        # Manifest do Behavior Pack
    └── scripts/
        └── main.js          # Script de integração com Bedrock
```

## 🔧 Requisitos

- **CMake** 3.16+
- **Compilador C++** (GCC, Clang, MSVC)
- **Android NDK** (para build Android)
- **Node.js** e **npm** (para o Script API)

## 🚀 Build

### Linux
```bash
./build.sh linux Release
```

### Android
```bash
export ANDROID_NDK=/caminho/para/android-ndk
./build.sh android Release
```

### Windows (Cross-compile com MinGW)
```bash
./build.sh windows Release
```

### Limpar build anterior
```bash
./build.sh linux Release clean
```

## 📦 Instalação no Minecraft Bedrock

### Método 1: WebAssembly (Recomendado)
1. Compile a biblioteca para WebAssembly:
   ```bash
   emcc bedrock_lib.cpp -o bedrock_lib.wasm        -s EXPORTED_FUNCTIONS='["_bedrock_init","_bedrock_shutdown","_bedrock_get_version"]'        -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]'        -s MODULARIZE=1
   ```

2. Copie o arquivo `.wasm` para `bedrock_script/scripts/`

3. Atualize `main.js` para carregar o WASM

### Método 2: Android Native (Não oficial)
> ⚠️ **Aviso**: Este método requer modificação do APK e não é suportado oficialmente.

1. Compile a biblioteca para Android:
   ```bash
   export ANDROID_ABI=arm64-v8a
   ./build.sh android Release
   ```

2. Extraia o APK do Minecraft Bedrock

3. Adicione `libbedrock_lib.so` em `lib/arm64-v8a/`

4. Modifique o `AndroidManifest.xml` para carregar a biblioteca

5. Reassine o APK

### Método 3: Servidor Dedicado (BDS)
1. Compile para Linux:
   ```bash
   ./build.sh linux Release
   ```

2. Copie `libbedrock_lib.so` para o diretório do servidor

3. Use Node-API ou carregamento dinâmico para integrar

## 🎮 Uso no Script API

O arquivo `bedrock_script/scripts/main.js` demonstra como integrar a biblioteca com o Script API oficial do Minecraft Bedrock.

### Comandos no jogo:
- `!spawn <entidade>` - Spawna uma entidade no local do jogador
- `!libinfo` - Mostra informações da biblioteca

## 📚 API Disponível

### Inicialização
```cpp
int bedrock_init();
void bedrock_shutdown();
const char* bedrock_get_version();
```

### Entidades
```cpp
int bedrock_get_entity(int entityId, EntityData* outData);
int bedrock_set_entity_position(int entityId, float x, float y, float z);
int bedrock_damage_entity(int entityId, float amount, const char* damageSource);
int bedrock_spawn_entity(const char* entityType, float x, float y, float z);
```

### Itens
```cpp
int bedrock_register_item(const CustomItem* item);
int bedrock_get_item(int itemId, CustomItem* outItem);
```

### Mundo
```cpp
int bedrock_get_block(float x, float y, float z, char* outBlockName, int maxLen);
int bedrock_set_block(float x, float y, float z, const char* blockName);
int bedrock_execute_command(const char* command);
```

### Eventos
```cpp
typedef void (*EventCallback)(const char* eventName, const char* eventData);
int bedrock_register_event_callback(const char* eventName, EventCallback callback);
```

## ⚠️ Limitações

1. **Não é suportado oficialmente**: O Minecraft Bedrock não tem API C++ oficial para mods
2. **Android**: Requer root ou modificação do APK
3. **iOS/Console**: Não é possível carregar bibliotecas nativas
4. **Windows 10/11**: Pode ser possível via DLL injection (não recomendado)

## 🔗 Alternativas Oficiais

Para modding oficial no Bedrock, use:
- **Behavior Packs** (JSON)
- **Resource Packs** (JSON, texturas, modelos)
- **Script API** (JavaScript/TypeScript)
- **Molang** (expressões matemáticas)

## 📄 Licença

Este projeto é apenas para fins educacionais. Minecraft é marca registrada da Mojang Studios.
