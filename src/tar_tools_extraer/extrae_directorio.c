/* *
 * * @file extrae_directorio.c
 * * @author ISO-1-10
 * * @date 03/04/2023
 * * @brief Implementa extracción de directorio
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
#include <dirent.h>

#include "extrae_metodos.h"
#include "../lib/auxiliary_functions.h"

/**
 * @brief Extrae un directorio y los fichero regulares que contenga
 * @param fd_tar File Descriptor del tar
 * @param dir_filename nombre del directorio
 * @param mode permisos del directorio
 * @param uid propietario del directorio
 * @param gid grupo del directorio
 * 
 * @details El directorio solo contendrá los ficheros regulares de su primer nivel
*/
int extrae_dir(int fd_tar, char *dir_filename, char *mode, char *uid, char *gid)
{
  int ret_read_hd;
  DIR *fd_dir;

  struct c_header_gnu_tar header;
  long unsigned int fdat_filesize = 0;

  // Comprobar ya si existe dir, sino crearlo, si nada funciona entonces error
  if ((fd_dir = opendir(dir_filename)) != NULL)
  {
    printf("\n ** Directorio %s existe. ATENCION: Se escribirá dentro de él\n.", dir_filename);
  }
  else if (mkdir(dir_filename, 0) == -1) // crea dir
  {
    return E_DIR1;
  }

  set_permiss_uid_gid(dir_filename, mode, uid, gid); //  asigna permisos, propietario y grupo a dir

  // COMIENZA A LEER LOS HEADER del contenido del directorio
  if ((ret_read_hd = read_header_get_size(&header, &fdat_filesize, &fd_tar)) != 0) // Lee header, comprueba, y obtiene tamaño fichero.
  {
    return ret_read_hd;
  }

  while (strncmp(header.magic, "ustar ", 6) == 0 && strncmp(header.name, dir_filename, strlen(dir_filename)) == 0) // con el nombre comprueba si fichero esta dentro del dir
  {
    printf("Escribe contenido del dir\n");
    if (header.typeflag[0] == '0')  // solo ficheros regulares
    {
      extrae_fich_regular(fd_tar, header.name, fdat_filesize, header.mode, header.uid, header.gid);
    }

    // Leer header
    if ((ret_read_hd = read_header_get_size(&header, &fdat_filesize, &fd_tar)) != 0) // Lee header, comprueba, y obtiene tamaño fichero.
    {
      return ret_read_hd;
    }
  }
  closedir(fd_dir);


  return 0;
}
