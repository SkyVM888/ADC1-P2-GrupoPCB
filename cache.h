#ifndef CACHE_H
#define CACHE_H

#include <stdint.h>
#include <stdbool.h>

#define SETS_DIRECT 4
#define VIAS_DIRECT 1

#define SETS_ASOCIA 2
#define VIAS_ASOCIA 2

// Estructura de una línea de caché
typedef struct {
    bool valid;          // Bit de validez (true: ocupada, false: vacía)
    uint16_t tag;        // Etiqueta almacenada en decimal
    uint32_t last_used;  // Contador para LRU
} CacheLine;

// Estructura para registrar el resultado de cada acceso
typedef struct {
    uint16_t tag;        // Recibido en decimal
    uint8_t index;       // Recibido en decimal
    uint8_t offset;      // Recibido en decimal
    bool hit;            // true = HIT, false = MISS
    uint8_t Via_used;    // Vía utilizada (0 o 1)
    int replaced_block;  // Bloque/Tag reemplazado (-1 si no hubo reemplazo)
} AccessResult;

// Estructuras principales de las cachés
typedef struct {
    CacheLine sets[SETS_DIRECT][VIAS_DIRECT];
} DirectCache;

typedef struct {
    CacheLine sets[SETS_ASOCIA][VIAS_ASOCIA];
} AsociaCache;

// Prototipos de funciones
void init_direct_cache(DirectCache *cache);
void init_asocia_cache(AsociaCache *cache);

AccessResult access_direct_cache(DirectCache *cache, uint16_t tag, uint8_t index, uint8_t offset);
AccessResult access_asocia_cache(AsociaCache *cache, uint16_t tag, uint8_t index, uint8_t offset, uint32_t current_access);

#endif // CACHE_H