#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Definição de exportação para biblioteca compartilhada
#ifdef _WIN32
    #ifdef BEDROCKLIB_EXPORTS
        #define BEDROCK_API __declspec(dllexport)
    #else
        #define BEDROCK_API __declspec(dllimport)
    #endif
#else
    #define BEDROCK_API __attribute__((visibility("default")))
#endif

// Estrutura de dados para entidade
struct EntityData {
    int entityId;
    float x, y, z;
    float health;
    float maxHealth;
    const char* name;
};

// Estrutura para item customizado
struct CustomItem {
    int itemId;
    const char* identifier;
    const char* displayName;
    int maxStackSize;
    float damage;
    int durability;
};

// ==================== FUNÇÕES DA API ====================

// Inicializa a biblioteca
BEDROCK_API int bedrock_init();

// Finaliza a biblioteca
BEDROCK_API void bedrock_shutdown();

// Versão da API
BEDROCK_API const char* bedrock_get_version();

// ==================== ENTIDADES ====================

// Obtém dados de uma entidade pelo ID
BEDROCK_API int bedrock_get_entity(int entityId, EntityData* outData);

// Define posição de uma entidade
BEDROCK_API int bedrock_set_entity_position(int entityId, float x, float y, float z);

// Aplica dano a uma entidade
BEDROCK_API int bedrock_damage_entity(int entityId, float amount, const char* damageSource);

// Cria uma entidade no mundo
BEDROCK_API int bedrock_spawn_entity(const char* entityType, float x, float y, float z);

// ==================== ITENS ====================

// Registra um item customizado
BEDROCK_API int bedrock_register_item(const CustomItem* item);

// Obtém dados de um item pelo ID
BEDROCK_API int bedrock_get_item(int itemId, CustomItem* outItem);

// ==================== MUNDO ====================

// Obtém bloco em uma posição
BEDROCK_API int bedrock_get_block(float x, float y, float z, char* outBlockName, int maxLen);

// Define bloco em uma posição
BEDROCK_API int bedrock_set_block(float x, float y, float z, const char* blockName);

// Executa comando no mundo
BEDROCK_API int bedrock_execute_command(const char* command);

// ==================== EVENTOS ====================

// Callback para eventos
typedef void (*EventCallback)(const char* eventName, const char* eventData);

// Registra um callback para eventos
BEDROCK_API int bedrock_register_event_callback(const char* eventName, EventCallback callback);

// ==================== UTILIDADES ====================

// Log customizado
BEDROCK_API void bedrock_log(const char* level, const char* message);

// Obtém tempo do servidor (ticks)
BEDROCK_API long bedrock_get_server_time();

// Obtém número de jogadores online
BEDROCK_API int bedrock_get_player_count();

#ifdef __cplusplus
}
#endif

