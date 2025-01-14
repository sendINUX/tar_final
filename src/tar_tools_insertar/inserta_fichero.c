/* *
 * * @file inserta_fichero.c
 * * @author Telmo Sendino
 * * @date 29/03/2023
 * * @brief Implementa interfaz para inserción de ficheros
 * *
 * */
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

#include "inserta_metodos.h"
#include "../lib/tar_errors.h"
#include "../lib/seek_header.h"

/**
 * @brief Reconoce e inserta un fichero regular, fichero enlace o directorio al tar
 *
 * @param f_mytar Nombre del tar
 * @param f_dat Nombre del fichero a insertar
 *
 * @return Numero de ficheros en f_mytar (o codigo de error)
 *
 */
int inserta_fichero(char *f_mytar, char *f_dat)
{
    int n = -2;
    int fd_tarfile, contador_fich;
    off_t tar_file_size, tam;
    struct stat st_fdat;

    tam = 0;
    contador_fich = 0;

    //  f_dat y f_mytar no puede ser el mismo fichero
    if (strcmp(f_mytar, f_dat) == 0)
    {
        fprintf(stderr, " * Fichero a insertar (%s) y fichero destino (%s) es el mismo; no se vuelca *\n", f_dat, f_mytar);
        return E_DESCO;
    }

    // Se comprueba si f_dat existe o no
    if (lstat(f_dat, &st_fdat) == 0)
    {

        // Operaciones de apertura de f_mytar
        if ((fd_tarfile = open_gnu_tar(f_mytar, &tar_file_size, O_RDWR | O_CREAT)) < 0) // tmb comprueba si es multiplo de 10 KiB
            return fd_tarfile;                                                          // codigo de error

        // Busqueda de fichero en tar
        if (tar_file_size != 0)
        {
            tar_file_size = 0;
            printf("Tar no vacio\n ");
            /* Posiciono la cabeza lectora justo antes del EOF */
            contador_fich = seek_eof_last_header(fd_tarfile, &tar_file_size);
        }
        if (contador_fich < 0)
        {
            close(fd_tarfile);
            return contador_fich; // codigo de error
        }


        // Distincion de directorio, enlace o fichero a extraer
        if (S_ISDIR(st_fdat.st_mode))
        {
            printf("%s es un DIRECTORIO\n", f_dat);
            printf("Ejecutando << inserta_directorio >>");
            n = inserta_directorio(fd_tarfile, f_dat, &tar_file_size, &contador_fich);
        }
        else if (S_ISLNK(st_fdat.st_mode))
        {
            printf("%s es un ENLACE SIMBOLICO\n", f_dat);
            printf("Ejecutando << inserta_enlace_simb >>");
            n = inserta_enlace_simb(fd_tarfile, f_dat, &tar_file_size, &contador_fich);
        }
        else if (S_ISREG(st_fdat.st_mode))
        {
            printf("%s es un FICHERO\n", f_dat);
            printf("Ejecutando << inserta_fichero >>");
            n = inserta_fich_regular(fd_tarfile, f_dat, &tar_file_size, &contador_fich);
        }
        else
        {
            fprintf(stderr, "Fichero (%s) a insertar NO soportado. \n", f_dat);
            close(fd_tarfile);
            return E_DESCO;
        }
    }

    // Si funcionó inserción, escribe en tar EOF y padding.
    if (n >= 0)
    {
        // Write end tar archive entry (2x512 bytes with zeros)
        tam = WriteEndTarArchive(fd_tarfile);
        tar_file_size = tar_file_size + tam;

        // complete Tar file to multiple of 10KB size block
        tam = WriteCompleteTarSize((long)tar_file_size, fd_tarfile);
        tar_file_size = tar_file_size + tam;
        printf("\n\n\nTamano tar actual: %li \n\n\n", tar_file_size);
    }
    close(fd_tarfile);

    return n;
}
