#include <stdio.h>

#define TAM_LINEA 50 // Cada lina del archivo puede ocupar hasta 100 caracteres en memoria 

int main(int argc, char *argv[])  //recibe los Arg de la termina argc =cantidad de argumentos argv=lista de argumentos. .\main.exe datos.txt    argc=2 

//argv[0] = ".\main.exe"
//argv[1] = "datos.txt"




{
    FILE *archivo;  //variable que representa el archivo que vamos a abrir, puntero.

    char linea[TAM_LINEA]; // areglo de caracteres ahi se va a guardar una linea del archivo a la vez----- primera linea primer dato, y asi sucesivamente. 

    int direccion;
    char extra;

    int hay_direcciones = 0;  //bandera 


    // Verificar que se ingreso el archivo
    if (argc != 2)
    {
        printf("Error: debe ingresar el nombre del archivo.\n");
        return 1;
    }


    // Abrir archivo
    archivo = fopen(argv[1], "r");   //abrir los datos para la lectura datos.txt

    if (archivo == NULL)   //si el archivo esta vacio 
    {
        printf("Error: no se pudo abrir el archivo.\n");
        return 1;
    }

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // PRIMER RECORRIDO: VALIDAR TODO EL ARCHIVO
   

    while (fgets(linea, TAM_LINEA, archivo) != NULL) //lee una linea de archivo y la guarda en linea de archivo usando como maximo 100 caracteres,mientras haya linea. 
    {
        int resultado;

        resultado = sscanf(linea, " %d %c", &direccion, &extra); //sscanf sirve para leer datos de un texto y se pasan a linea ---- buscá un número entero y después buscá si hay algún carácter extra.
        




        // Linea vacia
        if (resultado == EOF)
        {
            continue;
        }


        // Si encontro algo que no sea solo un numero
        if (resultado != 1)
        {
            printf("Error: se encontro una entrada no valida.\n");

            fclose(archivo);

            return 1;
        }


        // Verificar rango 
        if (direccion < 0 || direccion > 65535)
        {
            printf("Error: direccion fuera de rango.\n");

            fclose(archivo);

            return 1;
        }


        hay_direcciones = 1;
    }


    // Verificar archivo vacio
    if (hay_direcciones == 0)
    {
        printf("Error: el archivo no contiene direcciones.\n");

        fclose(archivo);

        return 1;
    }

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // SEGUNDO RECORRIDO: PROCESAR DIRECCIONES
   

    rewind(archivo); // volver al inicio del arhivo por que ya hicimos el escaneo de que esta correcto. 


    while (fgets(linea, TAM_LINEA, archivo) != NULL)
    {
        int resultado;

        resultado = sscanf(linea, " %d %c", &direccion, &extra);


        // Ignorar lineas vacias
        if (resultado == EOF)
        {
            continue;
        }


        //  ya  todo es valido
        if (resultado == 1)
        {
            printf("Direccion: %d\n", direccion);

            /*
                Parte de Bryan

                

                
                

                
            */
        }
    }


    fclose(archivo);

    return 0;
}

//PS C:\Users\Carlo\OneDrive\Desktop\Progra Arqui> gcc main.c -o main
//PS C:\Users\Carlo\OneDrive\Desktop\Progra Arqui> .\main.exe datos.txt