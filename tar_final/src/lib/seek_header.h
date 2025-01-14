/* *
 * * @file seek_header.h
 * * @author Telmo Sendino
 * * @date 18/02/2023
 * * @brief Funciones que recorren el tar y posicionan la cabeza lectora.
 * *
 * */

#include "auxiliary_functions.h"

// Posiciona la cabeza lectora justo antes de los bloques EOF del tar. (tras el ultimo header)
int seek_eof_last_header(int fd_mytar, off_t *p_tar_filesize);

// Posiciona la cabeza lectora justo despues del header con nombre a buscar.
int seek_file_by_name(int fd_mytar, char *p_nombre_buscar, struct c_header_gnu_tar *header);

