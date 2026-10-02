#include <stdio.h>
#include "cache.h"
#include "cache.c"

int main(void) {
    DirectCache cache_dir;
    AsociaCache cache_aso;

    init_direct_cache(&cache_dir);
    init_asocia_cache(&cache_aso);

    // Ejemplo: Dirección 64 -> En Caché Directa: Tag = 1, Index = 0, Offset = 0 (en decimal)
    uint16_t tag_dir = 1;
    uint8_t index_dir = 0;
    uint8_t offset_dir = 0;

    AccessResult res_dir = access_direct_cache(&cache_dir, tag_dir, index_dir, offset_dir);
    
    printf("\n[Directa] | Tag: %u | Index: %u | Offset: %u | Estado: %s | Via: %u\n",
           res_dir.tag, res_dir.index, res_dir.offset,
           res_dir.hit ? "HIT" : "MISS", res_dir.Via_used),"\n";

    // En Caché de 2 Vías para Dir 64: Tag = 2, Index = 0, Offset = 0 (en decimal)
    uint16_t tag_aso = 2;
    uint8_t index_aso = 0;
    uint8_t offset_aso = 0;

    AccessResult res_aso = access_asocia_cache(&cache_aso, tag_aso, index_aso, offset_aso, 1);

    printf("\n[Asociativa] | Tag: %u | Index: %u | Offset: %u | Estado: %s | Via: %u\n",
           res_aso.tag, res_aso.index, res_aso.offset,
           res_aso.hit ? "HIT" : "MISS", res_aso.Via_used,"\n");

    return 0;
}