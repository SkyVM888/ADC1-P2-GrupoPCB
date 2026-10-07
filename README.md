# Simulador de Memoria Caché de 16 bits | Arquitectura de Computadores

Repositorio oficial para el desarrollo del **Proyecto 2 de Arquitectura de Computadores I (GR20)** en el Tecnológico de Costa Rica (TEC). Consiste en un simulador modular desarrollado en **C11** para evaluar el desempeño de memorias caché con arquitectura de **Correspondencia Directa** y **Asociativa por Conjuntos (2 Vías)** mediante el cálculo del Tiempo Promedio de Acceso a Memoria (AMAT).

---

## 👥 Integrantes

| Nombre Completo | Carné | Usuario de GitHub |
| :--- | :---: | :---: |
| **Luis Pablo Vargas Muñoz** | `2021490483` | [@SkyVM888](https://github.com/SkyVM888) |
| **Carlos Daniel Hurtado Villalobos** | `2021067615` | [@Charlespixel](https://github.com/Charlespixel) |
| **Brayan Ariel Vargas Rojas** | `2021028522` | [@BrayanVR21](https://github.com/BrayanVR21) |

---

## 🛠️ Estructura del Proyecto

* **`main.c`**: Punto de entrada del programa, orquestación de la ejecución e impresión de resultados.
* **`cache.c` / `cache.h`**: Lógica de simulación para caché directa y de 2 vías, definición de estructuras y política LRU.
* **`leer.c`**: Módulo para la apertura, lectura y extracción de direcciones desde archivos de texto.
* **`Pruebas/`**: Carpeta que contiene los casos de prueba y trazas de direcciones (`.txt`).

---

## 📐 Especificaciones de la Arquitectura Simulada

* **Espacio de Direccionamiento:** 16 bits sin signo (64 KiB).
* **Capacidad Total de Caché:** 64 Bytes.
* **Tamaño de Bloque:** 16 Bytes.
* **Organizaciones Evaluadas:**
  * **Caché Directa:** 4 conjuntos, 1 vía por conjunto ($10\text{ bits Tag}, 2\text{ bits Index}, 4\text{ bits Offset}$).
  * **Caché 2 Vías:** 2 conjuntos, 2 vías por conjunto ($11\text{ bits Tag}, 1\text{ bit Index}, 4\text{ bits Offset}$).
* **Política de Reemplazo:** LRU (*Least Recently Used*) para la organización de 2 vías.
* **Métrica de Rendimiento (AMAT):** 
  $$\text{AMAT} = 1 + \left(\frac{\text{Fallos}}{\text{Accesos Totales}}\right) \times 20\text{ ciclos}$$

---

## ⚙️ Compilación y Ejecución

### Prerrequisitos
* Compilador **GCC 15.2.0** (Built by MSYS2 project) o superior con soporte para **C11**.

Para verificar la versión en tu sistema:
```bash
gcc --version
```
### Compilación

Compila todos los módulos fuente utilizando las banderas de advertencia estándar:

```bash
gcc -std=c11 -Wall -Wextra -o main.exe main.c    
```

### Ejecución

Para ejecutar el simulador pasando como parámetro la ruta de un archivo de prueba ubicado en la carpeta `pruebas/`:

```bash
./main.exe pruebas/{nombre_del_archivo}.txt
```
**Ejemplo de ejecución:**

```bash
./main.exe pruebas/conflicto.txt
```

## 📄 Archivos de Entrada (Pruebas/*.txt)

Los archivos de prueba deben incluir direcciones numéricas en formato decimal (ej. 0, 32) separadas por saltos de línea.

```bash
0
32
64
0
0
96
```

## 📊 Formato de Salida


El programa despliega en consola:

1. Traza Paso a Paso: Muestra para cada acceso la dirección enviada, sus campos extraídos (Tag, Index, Offset), el resultado del acceso (HIT / MISS) y la vía afectada o reemplazada.

2. Resumen de Métricas: Tabla comparativa final entre la caché directa y la de 2 vías con total de accesos, aciertos, fallos, Hit Rate, Miss Rate y valor de AMAT.