#include <stdio.h>
#include "cache.h"

#define TAM_LINEA 50

// Constantes para el cálculo del AMAT (coincidentes con tu ejemplo visual)
#define HIT_TIME 1.0
#define MISS_PENALTY 20.0

int main(int argc, char *argv[]) {
    FILE *archivo;
    char linea[TAM_LINEA];
    int direccion;
    char extra;
    int hay_direcciones = 0;

    // Verificar que se ingresó el archivo por argumento
    if (argc != 2) {
        printf("Error: debe ingresar el nombre del archivo. Ejemplo: ./main.exe datos.txt\n");
        return 1;
    }

    // Abrir archivo
    archivo = fopen(argv[1], "r");
    if (archivo == NULL) {
        printf("Error: no se pudo abrir el archivo '%s'.\n", argv[1]);
        return 1;
    }

    // ============================================================================
    // PRIMER RECORRIDO: VALIDAR TODO EL ARCHIVO
    // ============================================================================
    while (fgets(linea, TAM_LINEA, archivo) != NULL) {
        int resultado = sscanf(linea, " %d %c", &direccion, &extra);

        if (resultado == EOF || resultado == 0) {
            continue; 
        }

        if (resultado != 1) {
            printf("Error: se encontro una entrada no valida en el archivo.\n");
            fclose(archivo);
            return 1;
        }

        if (direccion < 0 || direccion > 65535) {
            printf("Error: direccion %d fuera de rango (0 - 65535).\n", direccion);
            fclose(archivo);
            return 1;
        }

        hay_direcciones = 1;
    }

    if (hay_direcciones == 0) {
        printf("Error: el archivo no contiene direcciones.\n");
        fclose(archivo);
        return 1;
    }

    // ============================================================================
    // ENCABEZADO GENERAL
    // ============================================================================
    printf("================================================================================\n");
    printf("SIMULACION DE MEMORIA CACHE - PROYECTO 2 (ADC1)\n");
    printf("================================================================================\n");

    // ============================================================================
    // SIMULACIÓN 1: CORRESPONDENCIA DIRECTA (1 VÍA)
    // ============================================================================
    rewind(archivo);
    DirectCache cache_dir;
    init_direct_cache(&cache_dir);

    int total_accesses = 0;
    int hits_dir = 0, misses_dir = 0;

    printf("\n--- ORGANIZACION 1: CORRESPONDENCIA DIRECTA (1 VIA) ---\n");

    while (fgets(linea, TAM_LINEA, archivo) != NULL) {
        int resultado = sscanf(linea, " %d %c", &direccion, &extra);
        if (resultado != 1) continue;

        total_accesses++;
        uint16_t tag_directa = direccion >> 6;
        uint8_t index_directa = (direccion >> 4) & 3;
        uint8_t offset_directa = direccion & 15;

        AccessResult res_dir = access_direct_cache(&cache_dir, tag_directa, index_directa, offset_directa);

        if (res_dir.hit) hits_dir++;
        else misses_dir++;

        char reemplazo_str[20];
        if (res_dir.replaced_block == -1) {
            sprintf(reemplazo_str, "Ninguno");
        } else {
            sprintf(reemplazo_str, "%d", res_dir.replaced_block);
        }

        printf("Acceso #%-2d | Dir: 0x%04X (%5d) | Tag: 0x%03X | Index: %d | Offset: %2d | %-4s | Via: %d | Reemplaza Bloque: %s\n",
               total_accesses,
               (unsigned int)direccion, direccion,
               (unsigned int)res_dir.tag,
               res_dir.index,
               res_dir.offset,
               res_dir.hit ? "HIT" : "MISS",
               res_dir.Via_used,
               reemplazo_str);
    }

    double hit_rate_dir = (total_accesses > 0) ? ((double)hits_dir / total_accesses) * 100.0 : 0.0;
    double miss_rate_dir = (total_accesses > 0) ? ((double)misses_dir / total_accesses) * 100.0 : 0.0;
    double miss_rate_dec_dir = (total_accesses > 0) ? (double)misses_dir / total_accesses : 0.0;
    double amat_dir = HIT_TIME + (miss_rate_dec_dir * MISS_PENALTY);

    printf("\n================================================================================\n");
    printf("RESUMEN DE SIMULACION: CORRESPONDENCIA DIRECTA\n");
    printf("================================================================================\n");
    printf("Total de Accesos     : %d\n", total_accesses);
    printf("Aciertos (Hits)      : %d\n", hits_dir);
    printf("Fallos (Misses)      : %d\n", misses_dir);
    printf("Tasa de Aciertos     : %.2f%%\n", hit_rate_dir);
    printf("Tasa de Fallos       : %.2f%%\n", miss_rate_dir);
    printf("AMAT                 : %.2f ciclos\n", amat_dir);
    printf("================================================================================\n");


    // ============================================================================
    // SIMULACIÓN 2: ASOCIATIVA POR CONJUNTOS (2 VÍAS - LRU)
    // ============================================================================
    rewind(archivo);
    AsociaCache cache_aso;
    init_asocia_cache(&cache_aso);

    int acceso_count = 0;
    int hits_aso = 0, misses_aso = 0;

    printf("\n\n--- ORGANIZACION 2: ASOCIATIVA POR CONJUNTOS (2 VIAS - LRU) ---\n");

    while (fgets(linea, TAM_LINEA, archivo) != NULL) {
        int resultado = sscanf(linea, " %d %c", &direccion, &extra);
        if (resultado != 1) continue;

        acceso_count++;
        uint16_t tag_asociativa = direccion >> 5;
        uint8_t index_asociativa = (direccion >> 4) & 1;
        uint8_t offset_asociativa = direccion & 15;

        AccessResult res_aso = access_asocia_cache(&cache_aso, tag_asociativa, index_asociativa, offset_asociativa, (uint32_t)acceso_count);

        if (res_aso.hit) hits_aso++;
        else misses_aso++;

        char reemplazo_aso_str[20];
        if (res_aso.replaced_block == -1) {
            sprintf(reemplazo_aso_str, "Ninguno");
        } else {
            sprintf(reemplazo_aso_str, "%d", res_aso.replaced_block);
        }

        printf("Acceso #%-2d | Dir: 0x%04X (%5d) | Tag: 0x%03X | Index: %d | Offset: %2d | %-4s | Via: %d | Reemplaza Bloque: %s\n",
               acceso_count,
               (unsigned int)direccion, direccion,
               (unsigned int)res_aso.tag,
               res_aso.index,
               res_aso.offset,
               res_aso.hit ? "HIT" : "MISS",
               res_aso.Via_used,
               reemplazo_aso_str);
    }

    double hit_rate_aso = (total_accesses > 0) ? ((double)hits_aso / total_accesses) * 100.0 : 0.0;
    double miss_rate_aso = (total_accesses > 0) ? ((double)misses_aso / total_accesses) * 100.0 : 0.0;
    double miss_rate_dec_aso = (total_accesses > 0) ? (double)misses_aso / total_accesses : 0.0;
    double amat_aso = HIT_TIME + (miss_rate_dec_aso * MISS_PENALTY);

    printf("\n================================================================================\n");
    printf("RESUMEN DE SIMULACION: ASOCIATIVA DE 2 VIAS\n");
    printf("================================================================================\n");
    printf("Total de Accesos     : %d\n", total_accesses);
    printf("Aciertos (Hits)      : %d\n", hits_aso);
    printf("Fallos (Misses)      : %d\n", misses_aso);
    printf("Tasa de Aciertos     : %.2f%%\n", hit_rate_aso);
    printf("Tasa de Fallos       : %.2f%%\n", miss_rate_aso);
    printf("AMAT                 : %.2f ciclos\n", amat_aso);
    printf("================================================================================\n");

    fclose(archivo);
    return 0;
}
