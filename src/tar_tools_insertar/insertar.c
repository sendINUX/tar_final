/**
 * @file     insertar.c
 * @author   grupo ISO-1-10
 * @date     10/04/2023
 * @brief    Main function to insert a file from to tar gnu format archive.
 * @details  It can insert a directory, regular files or symbolic files.
 *           The tool also keeps the original permisses of the file.
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#define error(codigo, mensaje)                            \
    {                                                     \
        fprintf(stderr, "ERROR %d: %s", codigo, mensaje); \
        exit(1);                                          \
    };

int main(int argc, char *argv[])
{
    extern int inserta_fichero(char *f_mytar, char *f_dat);

    char *archivo_entrada;
    char *tar_filename;
    char mensaje[256];
    int n;

    // Verificar uso correcto de la funcion
    if (argc != 3)
    {
        fprintf(stderr, "Uso: %s [fichero|directorio|enlace simb.]  [tarfile]\n", argv[0]);
        return 1;
    }

    // Argumentos del programa
    archivo_entrada = argv[1];
    tar_filename = argv[2];

    printf("Archivo de entrada: %s\n", archivo_entrada);
    printf("TAR de entrada: %s\n", tar_filename);

    n = inserta_fichero(tar_filename, archivo_entrada);

    if (n < 0) // Errores
    {
        if (n == -1)
        {
            sprintf(mensaje, "No se puede abrir o crear f_mytar (%s).\n", archivo_entrada);
            error(n, mensaje);
        }
        if (n == -2)
        {
            error(n, "No se puede abrir f_dat.\n");
        }
        if (n == -3)
        {
            sprintf(mensaje, "f_mytar (%s) no tiene el formato de gnu tar.\n", archivo_entrada);
            error(n, mensaje);
        }
        if (n == -4)
        {
            error(n, "Error al escribir entrada (header o datablock) de f_dat en tar\n");
        }
        if (n == -99)
        {
            error(n, "Ocurrió un error desconocido.\n");
        }
    }
    else
    {
        printf("Hay %d archivos en el archivo tar %s .\n", n, tar_filename);
        return 0;
    }
}
