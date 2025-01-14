/* *
 * * @file auxiliary_functions.c
 * * @author Telmo Sendino
 * * @date 20/03/2023
 * * @brief Metodos auxiliares usados en la inserción/extracción
 * *
 * */
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "auxiliary_functions.h"

/**
 * @brief Calcula el offset del bloque de datos
 * @param file_size Tamaño fichero
 * @param type_flag tipo de fichero
 */
int calc_data_offset(long unsigned int file_size, char type_flag)
{
  int avanza;
  avanza = ((file_size % DATAFILE_BLOCK_SIZE) == 0) ? (file_size / DATAFILE_BLOCK_SIZE) * 512 : ((file_size / DATAFILE_BLOCK_SIZE) + 1) * 512;

  if ((type_flag == '5') | (type_flag == '2'))
  {
    printf(" ** type_flag=%c ** (header sin dataBlock, no avanza)\n", type_flag);
    avanza = 0;
  }
  return avanza;
}

/**
 * @brief Lee, comprueba el header; obtiene header y escribe en p_file_size el tamaño del fichero
 *
 * @param p_header Puntero de variable destino header
 * @param p_file_size Escribe aqui el tamaño del fichero (header.size)
 * @param p_fd_tar Puntero de Descriptor de Fichero del tar
 *
 * @attention Cabeza lectora de p_fd_tar debe apuntar al INICIO DEL HEADER.
 * @return 0 funciono correctamente, o numero de error si HEADER invalido
 */
int read_header_get_size(struct c_header_gnu_tar *p_header, long unsigned int *p_file_size, int *fd_tar)
{
  // Lee el header
  if (read(*fd_tar, p_header, DATAFILE_BLOCK_SIZE) != DATAFILE_BLOCK_SIZE)
    return E_TARFORM;

  // name debe acabar en null
  if (p_header->name[99] != '\0')
  {
    fprintf(stderr, "\n  ** Error: header.name no acaba en caracter NULL. (tar corrupto) **\n");
    return E_TARFORM;
  }

  // Obtenemos campo size
  sscanf(p_header->size, "%011lo", p_file_size);
  return 0;
}

/**
 * @brief Abre un tar de formato posix
 * @param f_mytar nombre tar
 * @param tar_filesize variable donde escribe el tamaño del tar.
 * @param o_flag modo de acceso
 * @return fd_tar Descriptor archivo de tar.
 */
int open_gnu_tar(char *f_mytar, long int *tar_filesize, int o_flag)
{
  int fd_tarfile;
  struct stat st_var;

  if ((fd_tarfile = open(f_mytar, o_flag, 0600)) == -1)
  {
    fprintf(stderr, "No se pudo abrir fichero tar %s\n", f_mytar);
    return E_OPEN_TAR;
  }

  // Comprobacion de tamaño tar multiplo de 10 KiB
  if (fstat(fd_tarfile, &st_var) == -1)
  {
    fprintf(stderr, "Falló stat sobre fichero tar %s\n", f_mytar);
    return E_OPEN_TAR;
  }

  *tar_filesize = st_var.st_size;
  if (*tar_filesize % TAR_FILE_BLOCK_SIZE != 0)
  {
    close(fd_tarfile);
    return E_TARFORM;
  }

  return fd_tarfile;
}

/**
 * @brief Asigna permisos, propietario y grupo a un fichero
 * @param fd_fich File Descriptor de fichero.
 * @param header_mode Permisos, corresponde con header.mode
 * @param header_uid Propietario
 * @param header_gid  Grupo
 */
int fset_permiss_uid_gid(int fd_fich, char *header_mode, char *header_uid, char *header_gid)
{
  mode_t permisos;
  uid_t propietario;
  gid_t grupo;

  sscanf(header_mode, "%07o", &permisos);
  fchmod(fd_fich, permisos);

  sscanf(header_uid, "%07o", &propietario);
  sscanf(header_gid, "%07o", &grupo);
  fchown(fd_fich, propietario, grupo);

  return 0;
}

/**
 * @brief Asigna permisos, propietario y grupo a un fichero
 * @param f_filename Nombre de fichero
 * @param header_mode Permisos, corresponde con header.mode
 * @param header_uid Propietario
 * @param header_gid  Grupo
 */
int set_permiss_uid_gid(char *f_filename, char *header_mode, char *header_uid, char *header_gid)
{
  mode_t permisos;
  uid_t propietario;
  gid_t grupo;

  sscanf(header_mode, "%07o", &permisos);
  chmod(f_filename, permisos);

  sscanf(header_uid, "%07o", &propietario);
  sscanf(header_gid, "%07o", &grupo);
  chown(f_filename, propietario, grupo);

  return 0;
}
