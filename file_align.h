#ifndef _FILE_ALIGN_H
#define _FILE_ALIGN_H

#include <sys/stat.h>

void file_align(char *fin, char *filename, int pad);
off_t fsize(char *file);

#endif
