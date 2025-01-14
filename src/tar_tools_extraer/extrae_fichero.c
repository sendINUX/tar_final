/* *
 * * @file extrae_fichero.c
 * * @author Telmo Sendino
 * * @date 03/04/2023
 * * @brief Implementa interfaz para extracción de ficheros
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


#include "extrae_metodos.h"
#include "../lib/tar_errors.h"
#include "../lib/seek_header.h"

/**
 * @brief Reconoce y extrae un fichero regular, fichero enlace o directorio del tar
 * 
 * @param f_mytar Nombre del tar
 * @param f_dat Nombre del fichero a extraer
 * 
*/
int extrae_fichero(char *f_mytar, char *f_dat)
{
    int n = -1;
    int fd_tarfile, ret_stat, seek_out;
    struct c_header_gnu_tar ret_header;
    off_t tar_file_size;
    struct stat st_var;
    long int seek_file_size, fdat_filesize;

    if ((fd_tarfile = open_gnu_tar(f_mytar, &tar_file_size, O_RDONLY)) < 0) // tmb comprueba si es multiplo de 10 KiB
    {
        return fd_tarfile; // codigo de error
    }

    /* Comprobamos si el archivo tar es correcto */
    if (tar_file_size % TAR_FILE_BLOCK_SIZE != 0)
    {
        close(fd_tarfile);
        return E_TARFORM;
    }

    /* Posiciono la cabeza lectora al inicio del bloque de datos del fichero a extraer */
    seek_file_size = seek_file_by_name(fd_tarfile, f_dat, &ret_header);

    if (seek_file_size < 0)
    {
        close(fd_tarfile);
        return seek_file_size; // si es negativo son errores. (o no encontrado)
    }

    // ENCONTRO fichero:  Comprobamos el tipo.
    if (ret_header.typeflag[0] == '0') // fichero regular
    {
        n = extrae_fich_regular(fd_tarfile, f_dat, seek_file_size, ret_header.mode, ret_header.uid, ret_header.gid);
    }
    else if (ret_header.typeflag[0] == '5') // directorio
    {
        n = extrae_dir(fd_tarfile, f_dat, ret_header.mode, ret_header.uid, ret_header.gid);
    }
    else if (ret_header.typeflag[0] == '2') // enlace simbólico
    {
        n = extrae_enlace_simb(f_dat,ret_header.linkname);
    }
    else
    {
        fprintf(stderr, " ** Header incorrecto o tipo de archivo a extraer NO sorportado **\n");
        close(fd_tarfile);
        return E_TARFORM;
    }
    close(fd_tarfile);
    return n;
}
