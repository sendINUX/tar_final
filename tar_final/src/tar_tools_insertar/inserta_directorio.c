/* *
 * * @file inserta_directorio.c
 * * @author ISO-1-10
 * * @date 20/03/2023
 * * @brief Implementa la inserción de directorio
 * *
 * */

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
 * @brief Inserta un directorio y su contenido en el tar.
 * @param f_mytar nombre de archivo tar
 * @param f_dir nombre de directorio
 * @return num de archivos en f_mytar (tras la inserción) o un codigo de error.
 * 
 * @details Inserta SOLAMENTE los ficheros regulares del PRIMER nivel de f_dir.
 */
int inserta_directorio(int fd_tar, char *f_dir, off_t *tar_size, int *cont_fichero)
{
  DIR *fd_dir;

  long unsigned int b_header_file;
  struct c_header_gnu_tar my_tardat;
  struct stat stat_file_in_dir;
  struct dirent *fich_actual_dirent;

  char path[512];

  // Apertura del directorio
  if ((fd_dir = opendir(f_dir)) == NULL)
  {
    fprintf(stderr, "No se puede abrir el directorio %s\n", f_dir);
    return E_OPEN_DAT;
  }

  bzero(&my_tardat, sizeof(my_tardat));
  if (BuilTarHeader(f_dir, &my_tardat) != HEADER_OK) // genera el header del directorio
  {
    fprintf(stderr, "No se generado bien los datos del TarHeader\n");
    closedir(fd_dir);
    return E_DESCO;
  }

  // Añadido nuevo fichero (el directorio)
  b_header_file = write(fd_tar, &my_tardat, sizeof(my_tardat)); // inserta f_dir en el tar.
  *cont_fichero+= 1;
  *tar_size = *tar_size + b_header_file;

  printf("Numero de bytes escritos en el header %li \n", b_header_file);

  // INSERTAR contenido DEL DIRECTORIO
  while ((fich_actual_dirent = readdir(fd_dir)) != NULL)
  {

    // Ignorar los directorios "." y ".."
    if (strcmp(fich_actual_dirent->d_name, ".") == 0 || strcmp(fich_actual_dirent->d_name, "..") == 0)
    {
      // Si es el directorio actual o el padre, ignorar
      printf("ignora el directorio. (%s)", fich_actual_dirent->d_name);
    }
    else
    {
      sprintf(path, "%s/%s", f_dir, fich_actual_dirent->d_name);
      if (stat(path, &stat_file_in_dir) == -1)
      {
        closedir(fd_dir);
        return E_CREATDEST;
      }

      // solo inserta archivos regulares
      else if (S_ISREG(stat_file_in_dir.st_mode))
      {
        inserta_fich_regular(fd_tar,path,tar_size,cont_fichero); 
      }
    }
  }
  closedir(fd_dir);
  return *cont_fichero;
}
