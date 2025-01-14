/* *
 * * @file auxiliary_functions.h
 * * @author Telmo Sendino
 * * @date 20/03/2023
 * * @brief Metodos auxiliares usados en la inserción/extracción
 * *
 * */

#include "../lib/tar_errors.h"
#include "create_mytar.h"

// Calcula el offset (o numero de bloques de datos),
int calc_data_offset(long unsigned int file_size, char type_flag);

// Abre tar
int open_gnu_tar(char *f_mytar, long int *tar_filesize, int o_flag);

// Leer header, comprueba si se pudo leer y obtiene size
int read_header_get_size(struct c_header_gnu_tar *p_header, long unsigned int *p_file_size, int *fd_tar);

// Asigna permisos, propietario y grupo a fichero
int fset_permiss_uid_gid(int fd_fich, char *header_mode, char *header_uid, char *header_gid);
int set_permiss_uid_gid(char *f_filename, char *header_mode, char *header_uid, char *header_gid);