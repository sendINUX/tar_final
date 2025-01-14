/* *
 * * @file create_mytar.h
 * * @author M.A.
 * * @date 18/02/2023
 * * @brief First version of mytar
 * * @details  Funciones auxiliares para crear un archivo tar.
 * *
 * */

#include "s_mytarheader.h"
#include <sys/types.h>

//-----------------------------------------------------------------------------
// Return UserName (string) from uid (integer). See man 2 stat and man getpwuid
char * getUserName(uid_t uid);

//------------------------------------------------------------------------------
// Return GroupName (string) from gid (integer). See man 2 stat and man getgrgid
char * getGroupName(gid_t gid);

//----------------------------------------
// Return Mode type entry  (tar Mode type)
char mode_tar( mode_t Mode);


// ------------------------------------------------------------------------
// (1.0) Build my_tardat structure with FileName stat info (See man 2 stat)
int BuilTarHeader(char *FileName, struct c_header_gnu_tar *pTarHeader);

// ----------------------------------------------------------------
// (1.2) write the data file (blocks of 512 bytes)
unsigned long WriteFileDataBlocks(int fd_DataFile, int fd_TarFile);

// ----------------------------------------------------------------
// (2.1)Write end tar archive entry (2x512 bytes with zeros)
unsigned long WriteEndTarArchive( int fd_TarFile);

// ----------------------------------------------------------------
// (2.2) complete Tar file to  multiple of 10KB size block
unsigned long WriteCompleteTarSize( unsigned long TarActualSize,  int fd_TarFile);

// Verify Tar file zize to  multiple of 10KB size blocks
int  VerifyCompleteTarSize( unsigned long TarActualSize) ;
