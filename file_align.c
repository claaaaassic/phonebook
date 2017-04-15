#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "file_align.h"

#define MAX_LASTNAME 100

void file_align(char *fin, char *filename, int pad)
{
    char rbuf[MAX_LASTNAME], wbuf[pad];
    int i = 0;
    FILE *forig, *fout;
    forig = fopen(fin, "r");
    fout = fopen(filename, "w+");

    while (fgets(rbuf, sizeof(rbuf), forig)) {
        memset(wbuf, 0, sizeof(wbuf));

        while (rbuf[i] != '\n')
            i++;
        rbuf[i] = '\0';

        if(pad - strlen(rbuf) < 0)
            printf("Warning: The length of %s is longer than %d. \n", rbuf, pad);

        strncpy(wbuf, rbuf, strlen(rbuf));
        fwrite(wbuf, pad, sizeof(char), fout);
        i = 0;
    }

    fclose(forig);
    fclose(fout);
}

off_t fsize(char *file)
{
    struct stat st;
    stat(file, &st);

    return st.st_size;
}
