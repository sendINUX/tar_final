/**
 * @file inserta_fich_regular.c
 * @author   ISO-1-10
 * @date     10/02/2023
 * @brief    Inserta fichero regular en tar
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

#include "inserta_metodos.h"
#include "../lib/auxiliary_functions.h"

/**
 * @brief Inserta un archivo regular en un archivo tar.
 *
 * @param fd_tar Descriptor de fichero del tar
 * @param f_dat Nombre del fichero regular
 * @param tar_size Tamaño fichero (se actualiza)
 * @param cont_fichero Numero de ficheros en tar (se actualiza
 * 
 * @return Numero de ficheros en f_mytar (o codigo de error)
 */
int inserta_fich_regular(int fd_tar, char *f_dat, off_t *tar_size, int *cont_fichero)
{
  int fd_dat;
  long tam, b_header_dat;
  struct c_header_gnu_tar my_tardat;

  tam = 0;

  // Apertura de archivos
  if ((fd_dat = open(f_dat, O_RDONLY)) == -1)
  {
    fprintf(stderr, "No se puede abrir el fichero de datos %s\n", f_dat);
    return E_OPEN_DAT;
  }

  // Genera el header del archivo (en f_mytar)
  bzero(&my_tardat, sizeof(my_tardat));

  if (BuilTarHeader(f_dat, &my_tardat) != HEADER_OK)
  {
    fprintf(stderr, "No se generado bien los datos del TarHeader\n");
    close(fd_dat);
    return E_DESCO;
  }

  // Escribe header
  if ((b_header_dat = write(fd_tar, &my_tardat, sizeof(my_tardat))) == -1)
  {
    close(fd_dat);
    return E_INSERT_DAT;
  }
  *tar_size = *tar_size + b_header_dat;

  // Escribe bloque de datos
  if ((tam = WriteFileDataBlocks(fd_dat, fd_tar)) == -1)
  {
    close(fd_dat);
    return E_INSERT_DAT;
  }

  *tar_size = *tar_size + tam;
  *cont_fichero = *cont_fichero + 1;

  close(fd_dat);
  return *cont_fichero;
}