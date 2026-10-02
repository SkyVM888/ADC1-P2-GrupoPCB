#include "cache.h"

// ============================================================================
// Inicialización
// ============================================================================

// Inicializa la Caché Directa (todas las líneas inválidas)
void init_direct_cache(DirectCache *cache) {
    for (int s = 0; s < SETS_DIRECT; s++) {
        for (int v = 0; v < VIAS_DIRECT; v++) {
            cache->sets[s][v].valid = false;
            cache->sets[s][v].tag = 0;
            cache->sets[s][v].last_used = 0;
        }
    }
}

// Inicializa la Caché de 2 Vías (todas las líneas inválidas)
void init_asocia_cache(AsociaCache *cache) {
    for (int s = 0; s < SETS_ASOCIA; s++) {
        for (int v = 0; v < VIAS_ASOCIA; v++) {
            cache->sets[s][v].valid = false;
            cache->sets[s][v].tag = 0;
            cache->sets[s][v].last_used = 0;
        }
    }
}

// ============================================================================
// Simulación de Accesos a Caché
// ============================================================================

// Simulación de Caché de Correspondencia Directa recibiendo Tag, Index y Offset en decimal
AccessResult access_direct_cache(DirectCache *cache, uint16_t tag, uint8_t index, uint8_t offset) {
    AccessResult res;
    res.tag = tag;
    res.index = index;
    res.offset = offset;
    res.Via_used = 0;
    res.replaced_block = -1;

    CacheLine *line = &cache->sets[index][0];

    if (line->valid && line->tag == tag) {
        res.hit = true;
    } else {
        res.hit = false;
        if (line->valid) {
            res.replaced_block = line->tag; // Guarda la etiqueta del bloque expulsado
        }
        line->valid = true;
        line->tag = tag;
    }

    return res;
}

// Simulación de Caché Asociativa de 2 Vías con reemplazo LRU recibiendo parámetros en decimal
AccessResult access_asocia_cache(AsociaCache *cache, uint16_t tag, uint8_t index, uint8_t offset, uint32_t current_access) {
    AccessResult res;
    res.tag = tag;
    res.index = index;
    res.offset = offset;
    res.replaced_block = -1;

    CacheLine *set = cache->sets[index];

    // 1. Verificación de Acierto (HIT)
    for (int v = 0; v < VIAS_ASOCIA; v++) {
        if (set[v].valid && set[v].tag == tag) {
            res.hit = true;
            res.Via_used = v;
            set[v].last_used = current_access; // Actualiza el uso para LRU
            return res;
        }
    }

    // 2. Si es FALLO (MISS), buscar vía inválida (vacía)
    res.hit = false;
    for (int v = 0; v < VIAS_ASOCIA; v++) {
        if (!set[v].valid) {
            res.Via_used = v;
            set[v].valid = true;
            set[v].tag = tag;
            set[v].last_used = current_access;
            return res;
        }
    }

    // 3. Si el conjunto está lleno, aplicar reemplazo LRU (expulsar la vía menos reciente)
    int lru_way = 0;
    if (set[1].last_used < set[0].last_used) {
        lru_way = 1;
    }

    res.Via_used = lru_way;
    res.replaced_block = set[lru_way].tag; // Se registra la etiqueta reemplazada

    // Insertar el nuevo bloque
    set[lru_way].tag = tag;
    set[lru_way].last_used = current_access;

    return res;
}