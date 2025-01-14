/**
 * @file     extraer.c
 * @author   grupo ISO-1-10
 * @date     10/02/2023
 * @brief    Main function to extrar a file from a tar archive
 * @details  It can extract a directory, regular files or symbolic files from a
 *           gnu tar file format.
 *           The tool also recorvers the original permisses of the file.
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define error(codigo, mensaje)                              \
    {                                                       \
        fprintf(stderr, "\nERROR %d: %s", codigo, mensaje); \
        exit(1);                                            \
    };

int main(int argc, char *argv[])
{
    extern int extrae_fichero(char *f_mytar, char *f_dat);

    int n;
    char *fichero_extraer;
    char *tar_filename;
    char mensaje[256];

    if (argc != 3)
    {
        fprintf(stderr, "Uso: %s [fichero|directorio|enlace simb.]  [tarfile]\n", argv[0]);
        return 1;
    }

    fichero_extraer = argv[1];
    tar_filename = argv[2];

    printf("\n  - f_mytar: %s\n", argv[2]);                   // f_mytar
    printf("  - f_dat (archivo a extraer): %s\n\n", argv[1]); // f_dat (fich)

    n = extrae_fichero(tar_filename, fichero_extraer);

    if (n < 0) // Errores
    {
        if (n == -1)
        {
            sprintf(mensaje, "No se puede abrir f_mytar (%s).\n", tar_filename); 
            error(n, mensaje);
        }

        if (n == -3)
        {
            sprintf(mensaje, "f_mytar (%s) no tiene el formato de gnu tar.\n", tar_filename);
            error(n, mensaje);
        }
        if (n == -2)
        {
            sprintf(mensaje, "f_mytar (%s) no contiene el fichero de nombre f_dat (%s) \n", tar_filename, fichero_extraer);
            error(n, mensaje);
        }

        if (n == -4)
        {
            sprintf(mensaje, "No se puede crear/extraer el fichero f_dat (%s). \n", fichero_extraer);
            error(n, mensaje);
        }
        if (n == -5)
        {
            sprintf(mensaje, "No se puede crear la carpeta para extraer un directorio (%s). \n", fichero_extraer);
            error(n, mensaje);
        }
    }
    else
    {
        printf("\n\nArchivo %s extraido satisfactoriamente\n", fichero_extraer);
    }
}
