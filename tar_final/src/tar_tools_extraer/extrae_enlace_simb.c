/* *
 * * @file extrae_enlace_simb.c
 * * @author ISO-1-10
 * * @date 01/04/2023
 * * @brief Implementa extraer enlace simbolico
 * *
 * */

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "extrae_metodos.h"
#include "../lib/auxiliary_functions.h"

/**
 * @brief Extrae enlace simbolico del tar
 * 
 * @param name Nombre del enlace (header->name)
 * @param linkname_target Fichero destino del enlace (header->linkname)
 */
int extrae_enlace_simb(char *name, char *linkname_target)
{
  printf("\nNombre enlace (%s) -> enlaza a (%s)\n",name,linkname_target);
  sleep(0.2);
  if (symlink(linkname_target, name) == -1)
  {
    fprintf(stderr, "  * Falló función enlazar... *\n");
    return E_CREATDEST;
  }

  return 0;
}
