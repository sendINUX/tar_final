/* *
 * * @file seek_header_fun.c
 * * @author ISO-1-10
 * * @date 05/03/2023
 * * @brief Funciones que recorren el tar y posicionan la cabeza lectora.
 * *
 * */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "seek_header.h"

/**
 * @brief Encuentra posicion para insertar el ultimo header
 * @param fd_mytar File Descriptor del tar.
 * @param p_tar_filesize se escribe aqui el tamaño del tar
 * @return contador_fich ficheros en el tar (o integer negativo, si hubo error)
 *
 * @details Recorre el tar y desplaza la cabeza lectora hasta el inicio del EOF del tar.
 */
int seek_eof_last_header(int fd_mytar, off_t *p_tar_filesize)
{
  struct c_header_gnu_tar header;
  long int filesize, avanza;
  int contador_fich, ret_header;

  contador_fich = 0;

  // COMIENZA A LEER LOS HEADER del contenido del directorio
  if ((ret_header = read_header_get_size(&header, &filesize, &fd_mytar)) != 0) // Lee header, comprueba, y obtiene tamaño fichero.
  {
    return ret_header;
  }

  *p_tar_filesize = *p_tar_filesize + 512;
  printf("\nTamaño archivo: %li   | Nombre archivo %s\n", filesize, header.name);
  while (strncmp(header.magic, "ustar ", 6) == 0)
  {
    // Se incrementa el contador de ficheros
    contador_fich++;

    /* Calcula el offset del bloque de datos a avanzar */
    avanza = calc_data_offset(filesize, header.typeflag[0]);
    lseek(fd_mytar, avanza, SEEK_CUR); // avanza al siguiente header
    *p_tar_filesize = *p_tar_filesize + avanza;

    // Lee proximo header
    if ((ret_header = read_header_get_size(&header, &filesize, &fd_mytar)) != 0) // Lee header, comprueba, y obtiene tamaño fichero.
    {
      return ret_header;
    }

    *p_tar_filesize = *p_tar_filesize + 512;

    printf("\nTamaño archivo: %li   | Nombre archivo %s\n", filesize, header.name);
  }

  lseek(fd_mytar, -DATAFILE_BLOCK_SIZE, SEEK_CUR);
  *p_tar_filesize = *p_tar_filesize - 512; // tras leer el primer bloque de EOF, regresa al final de la ultima entrada del tar.

  printf("archivos contados: %d\n", contador_fich);
  return contador_fich;
}


/**
 * @brief Encuentra header de fichero a extraer
 * @param fd_mytar File Descriptor de tar
 * @param p_nombre_buscar Nombre fichero a extraer
 * @param header escribe aqui el header del fichero encontrado
 *
 * @return tamaño de fichero (o codigo de error)
 *
 */
int seek_file_by_name(int fd_mytar, char *p_nombre_buscar, struct c_header_gnu_tar *header)
{
  long int file_size, avanza;
  int ret_header;

  //  PRIMERA ITERACION
  if ((ret_header = read_header_get_size(header, &file_size, &fd_mytar)) != 0) // Lee header, comprueba, y obtiene tamaño fichero.
  {
    return ret_header;
  }
  printf("Buscando archivo...\n Tamaño archivo: %li   | Nombre archivo %s\n", file_size, header->name);

  while (strncmp(header->magic, "ustar ", 6) == 0 && strcmp(header->name, p_nombre_buscar) != 0)
  {
    // Avanza al siguiente header
    avanza = calc_data_offset(file_size, header->typeflag[0]);
    lseek(fd_mytar, avanza, SEEK_CUR);

    // Lee header
    if ((ret_header = read_header_get_size(header, &file_size, &fd_mytar)) != 0)
    {
      return ret_header;
    }
    printf("\n Tamaño archivo: %ld | Nombre archivo %s\n", file_size, header->name);
  }
  
  // Comprobamos por que salió del bucle
  if (strcmp(header->name, p_nombre_buscar) == 0)
  {
    return file_size; // archivo encontrado
  }
  else if (strcmp(header->name, "") == 0)
  {
    return E_NOEXIST;
  }
  else if (strncmp(header->magic, "ustar ", 6) == 0)
  {
    fprintf(stderr, "  **Error: magic invalido");
    return E_TARFORM;
  }
  else
  {
    return E_DESCO;
  }
}