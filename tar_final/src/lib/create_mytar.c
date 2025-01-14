/* *
 * * @file create_mytar.c
 * * @author M.A.
 * * @date 18/02/2023
 * * @brief First version of mytar
 * * @details  Create a tar file with only one "data file"
 * *
 * */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

#include "create_mytar.h"

#define HEADER_OK 1
#define HEADER_ERR 2

struct passwd *pws;
struct group *grp;

char FileDataBlock[DATAFILE_BLOCK_SIZE];

//-----------------------------------------------------------------------------
// Return UserName (string) from uid (integer). See man 2 stat and man getpwuid
char *getUserName(uid_t uid)
{
   pws = getpwuid(uid);
   return pws->pw_name;
}

//------------------------------------------------------------------------------
// Return GroupName (string) from gid (integer). See man 2 stat and man getgrgid
char *getGroupName(gid_t gid)
{
   grp = getgrgid(gid);
   return grp->gr_name;
}

//----------------------------------------
// Return Mode type entry  (tar Mode type)
char mode_tar(mode_t Mode)
{
   if (S_ISREG(Mode))
      return '0';

   if (S_ISLNK(Mode))
      return '2';
   if (S_ISCHR(Mode))
      return '3';
   if (S_ISBLK(Mode))
      return '4';
   if (S_ISDIR(Mode))
      return '5';
   if (S_ISFIFO(Mode))
      return '6';
   if (S_ISSOCK(Mode))
      return '7';
   return '0';
}

// ------------------------------------------------------------------------
// (1.0) Build my_tardat structure with FileName stat info (See man 2 stat)
int BuilTarHeader(char *FileName, struct c_header_gnu_tar *pTarHeader)
{
   struct stat stat_file;

   ssize_t Symlink_Size;
   int i;
   char *pTarHeaderBytes;
   unsigned int Checksum;

   bzero(pTarHeader, sizeof(struct c_header_gnu_tar));

   if (lstat(FileName, &stat_file) == -1)
      return HEADER_ERR;

   strcpy(pTarHeader->name, FileName);
   sprintf(pTarHeader->mode, "%07o", stat_file.st_mode & 07777);                 // Only  the least significant 12 bits
   sprintf(pTarHeader->uid, "%07o", stat_file.st_uid);
   sprintf(pTarHeader->gid, "%07o", stat_file.st_gid);

   if (S_ISLNK(stat_file.st_mode))
      sprintf(pTarHeader->size, "%011lo", (long unsigned int) 0);
   else
      sprintf(pTarHeader->size, "%011lo", stat_file.st_size);
   sprintf(pTarHeader->mtime, "%011lo", stat_file.st_mtime);
   // checksum  the last     sprintf(pTarHeader->checksum, "%06o", Checksum);

   pTarHeader->typeflag[0] = mode_tar(stat_file.st_mode);

   //  linkname
   if (S_ISLNK(stat_file.st_mode))
      Symlink_Size = readlink(FileName, pTarHeader->linkname, 100);

   strncpy(pTarHeader->magic, "ustar ", 6); // "ustar" followed by a space (without null char)
   strcpy(pTarHeader->version, " ");        //   space character followed by a null char.
   strcpy(pTarHeader->uname, getUserName(stat_file.st_uid));
   strcpy(pTarHeader->gname, getGroupName(stat_file.st_gid));
   //  devmayor (not used)
   //  devminor (not used)
   sprintf(pTarHeader->atime, "%011lo", stat_file.st_atime);
   sprintf(pTarHeader->ctime, "%011lo", stat_file.st_ctime);
   //  offset (not used)
   //  longnames (not used)
   //  unused (not used)
   //  sparse (not used)
   //  isextended (not used)
   //  realsize (not used)
   //  pad (not used)

   // compute checksum (the last)
   memset(pTarHeader->checksum, ' ', 8); // Initialize to blanc spaces
   pTarHeaderBytes = (unsigned char *)pTarHeader;

   for (i = 0, Checksum = 0; i < sizeof(struct c_header_gnu_tar); i++)
      Checksum = Checksum + pTarHeaderBytes[i];

   sprintf(pTarHeader->checksum, "%06o", Checksum); // six octal digits followed by a null and a space character

   return HEADER_OK;
}

// ----------------------------------------------------------------
// (1.2) write the data file (blocks of 512 bytes)
unsigned long WriteFileDataBlocks(int fd_DataFile, int fd_TarFile)
{
   unsigned long num_write_bytes;
   int n;

   // write the data file (blocks of 512 bytes)
   num_write_bytes = 0;
   printf("Datos Escritos :"); // Traza
   bzero(FileDataBlock, sizeof(FileDataBlock));
   // memset(FileDataBlock, '\0', sizeof(FileDataBlock));  // Similar a bzero(...)
   while ((n = read(fd_DataFile, FileDataBlock, sizeof(FileDataBlock))) > 0)
   {
      write(fd_TarFile, FileDataBlock, sizeof(FileDataBlock));
      num_write_bytes = num_write_bytes + sizeof(FileDataBlock); // ojo!!!, no se escriben n
      printf("--%d -", n);                                   // Traza
      bzero(FileDataBlock, sizeof(FileDataBlock));
   }

   printf("\n Total :Escritos %ld \n", num_write_bytes); // Traza
   return num_write_bytes;
}

// ----------------------------------------------------------------
// (2.1)Write end tar archive entry (2x512 bytes with zeros)
unsigned long WriteEndTarArchive(int fd_TarFile)
{
   unsigned long num_write_bytes;
   int i;
   int n = 2;

   // write end tar archive entry (2x512 bytes with zeros)
   num_write_bytes = 0;
   // To Do...

   bzero(FileDataBlock, 512);

   for (i = 1; i <= n; i++)
   {
      write(fd_TarFile, FileDataBlock, 512);
      num_write_bytes = num_write_bytes + 512;
   }

   printf(" Escritos (End block)%d total %ld\n", n, num_write_bytes); // Traza

   return num_write_bytes;
}

// ----------------------------------------------------------------
// (2.2) complete Tar file to  multiple of 10KB size block
unsigned long WriteCompleteTarSize(unsigned long TarActualSize, int fd_TarFile)
{
   unsigned long num_write_bytes;
   unsigned long Module, offset, offset_excess, offset_count;
   int i;

   num_write_bytes = 0;
   // complete to  multiple of 10KB size blocks
   printf("TAR_FILE_BLOCK_SIZE=%ld  TarFileSize=%ld\n", TAR_FILE_BLOCK_SIZE, TarActualSize); // Traza

   Module = TarActualSize % 10240;

   // Si ya es multiplo no escribimos padding
   if (Module == 0)
   {
      printf("OK: Generado el EndTarBlocks del archivo tar %ld bytes \n", num_write_bytes); // Traza
      return 0;
   }

   offset = 10240 - Module;

   offset_count = offset / 512;
   offset_excess = offset % 512;

   bzero(FileDataBlock, 512);

   for (i = 0; i <= offset_count; i++)
   {
      if (!(i == offset_count))
      {
         write(fd_TarFile, FileDataBlock, 512);
         num_write_bytes = num_write_bytes + 512;
      }
      else
      {
         write(fd_TarFile, FileDataBlock, offset_excess);
         num_write_bytes = num_write_bytes + offset_excess;
      }
   }

   printf("OK: Generado el EndTarBlocks del archivo tar %ld bytes \n", num_write_bytes); // Traza

   return offset;
}

// Verify Tar file zize to  multiple of 10KB size blocks
int VerifyCompleteTarSize(unsigned long pTarActualSize)
{

   // Verify
   if ((pTarActualSize % TAR_FILE_BLOCK_SIZE) != 0)
   {
      fprintf(stderr, "Error al generar el fichero tar. Tamanio erroneo %ld\n", pTarActualSize);
      return ERROR_GENERATE_TAR_FILE2;
   }
   return 0;
}
