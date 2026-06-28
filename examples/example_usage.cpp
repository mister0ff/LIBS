#include "bedrock_lib.hpp"
#include <cstdio>
#include <cstring>

// Callback de exemplo para eventos
void onPlayerJoin(const char* eventName, const char* eventData) {
    printf("[EXEMPLO] Evento: %s\n", eventName);
    printf("[EXEMPLO] Dados: %s\n", eventData);
}

int main() {
    printf("=== Exemplo de Uso da Bedrock Native Library ===\n\n");

    // 1. Inicializar a biblioteca
    printf("1. Inicializando biblioteca...\n");
    if (bedrock_init() != 1) {
        printf("Erro ao inicializar!\n");
        return 1;
    }
    printf("   OK!\n\n");

    // 2. Obter versão
    printf("2. Versão da biblioteca:\n");
    printf("   %s\n\n", bedrock_get_version());

    // 3. Spawnar uma entidade
    printf("3. Spawnando entidade...\n");
    int entityId = bedrock_spawn_entity("minecraft:zombie", 100.0f, 64.0f, 200.0f);
    printf("   Entidade spawnada com ID: %d\n\n", entityId);

    // 4. Obter dados da entidade
    printf("4. Obtendo dados da entidade...\n");
    EntityData ent;
    if (bedrock_get_entity(entityId, &ent) == 1) {
        printf("   Nome: %s\n", ent.name);
        printf("   Posição: (%.1f, %.1f, %.1f)\n", ent.x, ent.y, ent.z);
        printf("   Vida: %.1f/%.1f\n\n", ent.health, ent.maxHealth);
    }

    // 5. Aplicar dano
    printf("5. Aplicando dano...\n");
    bedrock_damage_entity(entityId, 5.0f, "player_attack");
    printf("   Dano aplicado!\n\n");

    // 6. Registrar um item customizado
    printf("6. Registrando item customizado...\n");
    CustomItem item;
    item.itemId = 0; // Será atribuído pela biblioteca
    item.identifier = "custom:fire_sword";
    item.displayName = "Espada de Fogo";
    item.maxStackSize = 1;
    item.damage = 12.5f;
    item.durability = 1561;

    int itemId = bedrock_register_item(&item);
    printf("   Item registrado com ID: %d\n\n", itemId);

    // 7. Obter dados do item
    printf("7. Verificando item registrado...\n");
    CustomItem retrievedItem;
    if (bedrock_get_item(itemId, &retrievedItem) == 1) {
        printf("   Identificador: %s\n", retrievedItem.identifier);
        printf("   Nome: %s\n", retrievedItem.displayName);
        printf("   Dano: %.1f\n", retrievedItem.damage);
        printf("   Durabilidade: %d\n\n", retrievedItem.durability);
    }

    // 8. Executar comando
    printf("8. Executando comando...\n");
    bedrock_execute_command("say Biblioteca nativa carregada com sucesso!");
    printf("   Comando executado!\n\n");

    // 9. Registrar callback de evento
    printf("9. Registrando callback de evento...\n");
    bedrock_register_event_callback("playerJoin", onPlayerJoin);
    printf("   Callback registrado!\n\n");

    // 10. Log
    printf("10. Testando log...\n");
    bedrock_log("INFO", "Este é um log de informação");
    bedrock_log("WARN", "Este é um log de aviso");
    bedrock_log("ERROR", "Este é um log de erro");
    printf("   Logs escritos!\n\n");

    // 11. Finalizar
    printf("11. Finalizando biblioteca...\n");
    bedrock_shutdown();
    printf("   OK!\n\n");

    printf("=== Exemplo concluído! ===\n");
    return 0;
}

