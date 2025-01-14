/* *
 * * @file extrae_fich_regular.c
 * * @author grupo ISO-1-10
 * * @date 03/04/2023
 * * @brief Implementa extracción fichero regular
 * *
 * */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <stdlib.h>

#include "extrae_metodos.h"
#include "../lib/auxiliary_functions.h"

/**
 * @brief Crea un fichero regular a leyendo el bloque de datos de un fichero regular de un tar.
 *
 * @param fd_tar File Descriptor de tar
 * @param ext_filename Nombre fichero a extraer
 * @param fdat_size Tamaño fichero
 * @param mode Permisos del fichero
 * @param uid Uid
 * @param gid Gid (corresponden con header->gid)
 *
 * @return 0 funciono correctamente, o numero de error
 */
int extrae_fich_regular(int fd_tar, char *ext_filename, long unsigned int fdat_size, char *mode, char *uid, char *gid)
{

  char file_data_block[DATAFILE_BLOCK_SIZE];
  int aux, fd_extracted_file, bytes_write_file;

  aux = fdat_size;

  bzero(file_data_block, sizeof(file_data_block));
  printf("Tamaño archivo salida %ld B \n    ### Escribiendo...  ", fdat_size);

  if ((fd_extracted_file = open(ext_filename, O_RDWR | O_CREAT | O_TRUNC)) == -1)
  {
    return E_CREATDEST;
  }

  while ((read(fd_tar, file_data_block, sizeof(file_data_block)) > 0) && aux > 512)
  {
    bytes_write_file = write(fd_extracted_file, file_data_block, sizeof(file_data_block)); // Escribe en bloques de datos
    printf("--%d -", bytes_write_file);
    bzero(file_data_block, sizeof(file_data_block));
    aux = aux - sizeof(file_data_block); // bytes pendientes de escribir
  }

  // Escribe los bytes restantes
  bytes_write_file = write(fd_extracted_file, file_data_block, aux);
  printf("--%d -", bytes_write_file);
  
  
  // Asignar permisos, propietario y grupo
  fset_permiss_uid_gid(fd_extracted_file,mode,uid,gid);

  close(fd_extracted_file);

  return 0;
}
