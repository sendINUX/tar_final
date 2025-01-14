/* *
 * * @file inserta_metodos.h
 * * @author Telmo Sendino
 * * @date 29/03/2023
 * * @brief Define metodos para la inserción de ficheros
 * *
 * */
#include <sys/types.h>


int inserta_fich_regular(int fd_tar, char *f_dat, off_t *tar_size, int *cont_fichero);
int inserta_directorio(int fd_tar, char *f_dir, off_t *tar_size, int *cont_fichero);
int inserta_enlace_simb(int fd_tar, char *f_enlace, off_t *tar_size, int *cont_fichero);
