/**
 * @file inserta_enlace_simb.c
 * @author   ISO-1-10
 * @date     10/02/2023
 * @brief    Implementa la inserción de enlace simbólico
 * 
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include "inserta_metodos.h"
#include "../lib/auxiliary_functions.h"

/**
 * @brief Inserta un ENLACE SIMBOLICO
 * @param fd_tar File Descritor del tar
 * @param f_enlace nombre de enlace simbolico
 * @param tar_size tamaño del archivo tar
 * @param cont_fichero numero de ficheros en el tar
 * 
 * @details tar_size y cont_fichero se actualizan tras inserción
 *
 * @return num de archivos en f_mytar (tras la inserción) o un codigo de error.
 */
int inserta_enlace_simb(int fd_tar, char *f_enlace, off_t *tar_size, int *cont_fichero)
{
    long unsigned int b_header_file;
    struct c_header_gnu_tar my_tardat;

   
    bzero(&my_tardat, sizeof(my_tardat));
    if (BuilTarHeader(f_enlace, &my_tardat) != HEADER_OK) // genera el header de enlace
    {
        fprintf(stderr, "No se generado bien los datos del TarHeader\n");
        return E_DESCO;
    }

    // inserta f_enlace en el tar.
    if ((b_header_file = write(fd_tar, &my_tardat, sizeof(my_tardat))) == -1)
    {
        fprintf(stderr, "Falló escritura de enlace (%s) en tar.\n", f_enlace);
        return E_DESCO;
    }

    *tar_size = *tar_size + b_header_file;
    *cont_fichero = *cont_fichero + 1;

    printf("Numero de bytes escritos en el header %li \n", b_header_file);

    return *cont_fichero;
}