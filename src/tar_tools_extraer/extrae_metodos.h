/* *
 * * @file extrae_metodos.h
 * * @author Telmo Sendino
 * * @date 03/04/2023
 * * @brief Define metodos para la extracción de ficheros
 * *
 * */
#include <sys/types.h>


int extrae_fich_regular(int fd_tar, char *ext_filename, long unsigned int fdat_size, char *mode, char *uid, char *gid);
int extrae_dir(int fd_tar, char *dir_filename, char *mode, char *uid, char *gid);
int extrae_enlace_simb(char *name, char *linkname_target);