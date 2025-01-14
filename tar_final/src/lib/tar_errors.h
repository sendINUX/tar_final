/* *
 * * @file tar_erros.c
 * * @author Telmo Sendino
 * * @date 01/04/2023
 * * @brief Codigos de error devolverá la utilidad tar
 * *
 * */


// for checking headers
#ifndef h_err
#define HEADER_OK 1
#define HEADER_ERR 2
#endif

// for insertion operation
#ifndef ins_errors
#define E_OPEN_TAR (-1)   // No se puede abrir o crear f_mytar.
#define E_OPEN_DAT (-2)   // No se puede abrir  f_dat.
#define E_TARFORM (-3) // f_dat no tiene el formato de gnu tar.
#define E_INSERT_DAT (-4) // Error al escribir entrada (header o datablock) de f_dat en tar
#define E_DESCO (-99)  // error desconocido
#endif

// for extraction operation
#ifndef ext_errors
#define HEADER_OK 1
#define HEADER_ERR 2

#define E_OPEN (-1)    // No se puede abrir f_mytar.
#define E_NOEXIST (-2)   // f_mytar no contiene el fichero de nombre f_dat.
#define E_TARFORM (-3) // f_mytar no tiene el formato de gnu tar.
#define E_CREATDEST (-4) // No se puede crear el archivo f_dat
#define E_DIR1 (-5) // no se pudo crear directorio
#endif

