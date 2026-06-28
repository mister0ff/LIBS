#include "bedrock_lib.hpp"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

// ==================== IMPLEMENTAÇÃO ====================

static bool g_initialized = false;
static std::map<std::string, EventCallback> g_eventCallbacks;
static std::map<int, CustomItem> g_customItems;
static int g_nextItemId = 10000;

// Simulação de entidades (em produção, isso viria do jogo)
static std::map<int, EntityData> g_entities;

BEDROCK_API int bedrock_init() {
    if (g_initialized) {
        return 0; // Já inicializado
    }

    // Inicialização da biblioteca
    g_initialized = true;

    // Log de inicialização
    bedrock_log("INFO", "Bedrock Native Library inicializada");
    bedrock_log("INFO", bedrock_get_version());

    return 1; // Sucesso
}

BEDROCK_API void bedrock_shutdown() {
    if (!g_initialized) {
        return;
    }

    g_eventCallbacks.clear();
    g_customItems.clear();
    g_entities.clear();
    g_initialized = false;

    bedrock_log("INFO", "Bedrock Native Library finalizada");
}

BEDROCK_API const char* bedrock_get_version() {
    return "Bedrock Native Library v1.0.0 - Build: " __DATE__ " " __TIME__;
}

// ==================== ENTIDADES ====================

BEDROCK_API int bedrock_get_entity(int entityId, EntityData* outData) {
    if (!g_initialized || outData == nullptr) {
        return -1;
    }

    // Em produção: chamar API interna do Minecraft
    // Simulação:
    auto it = g_entities.find(entityId);
    if (it != g_entities.end()) {
        *outData = it->second;
        return 1;
    }

    return 0; // Entidade não encontrada
}

BEDROCK_API int bedrock_set_entity_position(int entityId, float x, float y, float z) {
    if (!g_initialized) {
        return -1;
    }

    // Em produção: chamar API interna do Minecraft
    auto it = g_entities.find(entityId);
    if (it != g_entities.end()) {
        it->second.x = x;
        it->second.y = y;
        it->second.z = z;
        return 1;
    }

    return 0;
}

BEDROCK_API int bedrock_damage_entity(int entityId, float amount, const char* damageSource) {
    if (!g_initialized || damageSource == nullptr) {
        return -1;
    }

    // Em produção: aplicar dano via API do jogo
    char logMsg[256];
    snprintf(logMsg, sizeof(logMsg), "Dano de %.1f aplicado a entidade %d (fonte: %s)", 
             amount, entityId, damageSource);
    bedrock_log("DEBUG", logMsg);

    return 1;
}

BEDROCK_API int bedrock_spawn_entity(const char* entityType, float x, float y, float z) {
    if (!g_initialized || entityType == nullptr) {
        return -1;
    }

    // Em produção: spawnar via API do jogo
    static int nextEntityId = 1;
    int newId = nextEntityId++;

    EntityData ent;
    ent.entityId = newId;
    ent.x = x;
    ent.y = y;
    ent.z = z;
    ent.health = 20.0f;
    ent.maxHealth = 20.0f;
    ent.name = entityType;

    g_entities[newId] = ent;

    char logMsg[256];
    snprintf(logMsg, sizeof(logMsg), "Entidade %s spawnada em (%.1f, %.1f, %.1f) com ID %d", 
             entityType, x, y, z, newId);
    bedrock_log("INFO", logMsg);

    return newId;
}

// ==================== ITENS ====================

BEDROCK_API int bedrock_register_item(const CustomItem* item) {
    if (!g_initialized || item == nullptr) {
        return -1;
    }

    CustomItem newItem = *item;
    newItem.itemId = g_nextItemId++;

    g_customItems[newItem.itemId] = newItem;

    char logMsg[256];
    snprintf(logMsg, sizeof(logMsg), "Item registrado: %s (ID: %d)", 
             newItem.identifier, newItem.itemId);
    bedrock_log("INFO", logMsg);

    return newItem.itemId;
}

BEDROCK_API int bedrock_get_item(int itemId, CustomItem* outItem) {
    if (!g_initialized || outItem == nullptr) {
        return -1;
    }

    auto it = g_customItems.find(itemId);
    if (it != g_customItems.end()) {
        *outItem = it->second;
        return 1;
    }

    return 0;
}

// ==================== MUNDO ====================

BEDROCK_API int bedrock_get_block(float x, float y, float z, char* outBlockName, int maxLen) {
    if (!g_initialized || outBlockName == nullptr || maxLen <= 0) {
        return -1;
    }

    // Em produção: obter bloco do mundo
    strncpy(outBlockName, "minecraft:stone", maxLen - 1);
    outBlockName[maxLen - 1] = '\0';

    return 1;
}

BEDROCK_API int bedrock_set_block(float x, float y, float z, const char* blockName) {
    if (!g_initialized || blockName == nullptr) {
        return -1;
    }

    // Em produção: definir bloco no mundo
    char logMsg[256];
    snprintf(logMsg, sizeof(logMsg), "Bloco %s colocado em (%.1f, %.1f, %.1f)", 
             blockName, x, y, z);
    bedrock_log("DEBUG", logMsg);

    return 1;
}

BEDROCK_API int bedrock_execute_command(const char* command) {
    if (!g_initialized || command == nullptr) {
        return -1;
    }

    // Em produção: executar comando no servidor
    char logMsg[256];
    snprintf(logMsg, sizeof(logMsg), "Comando executado: %s", command);
    bedrock_log("INFO", logMsg);

    return 1;
}

// ==================== EVENTOS ====================

BEDROCK_API int bedrock_register_event_callback(const char* eventName, EventCallback callback) {
    if (!g_initialized || eventName == nullptr || callback == nullptr) {
        return -1;
    }

    g_eventCallbacks[eventName] = callback;

    char logMsg[256];
    snprintf(logMsg, sizeof(logMsg), "Callback registrado para evento: %s", eventName);
    bedrock_log("INFO", logMsg);

    return 1;
}

// Função interna para disparar eventos
void bedrock_fire_event(const char* eventName, const char* eventData) {
    auto it = g_eventCallbacks.find(eventName);
    if (it != g_eventCallbacks.end()) {
        it->second(eventName, eventData);
    }
}

// ==================== UTILIDADES ====================

BEDROCK_API void bedrock_log(const char* level, const char* message) {
    if (level == nullptr || message == nullptr) {
        return;
    }

    printf("[BEDROCK-LIB] [%s] %s\n", level, message);
    fflush(stdout);
}

BEDROCK_API long bedrock_get_server_time() {
    // Em produção: obter ticks do servidor
    return 0;
}

BEDROCK_API int bedrock_get_player_count() {
    // Em produção: contar jogadores online
    return 0;
}

