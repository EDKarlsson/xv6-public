#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
// #include "user.h"

void proc_ls(char *arg)
{
    int e = exec("ls", &arg);
}

void redirectLs(char *dir)
{

}

int openFile(char file[])
{
    // Attempt to open the destination file and exit if it fails
    // printf("Copying: %s to file: %s\n", argv[1], argv[2]);
    int fd = open(file, (O_CREAT | O_WRONLY | O_TRUNC), (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
    if(fd < 0) {
        fprintf(stderr, "Failed to open file %s\nExiting...\n", file);
        exit(1);
    }

    return fd;
}

int main(int argc, char * argv[])
{
     // Variable Declaration
    char buf[512];
    double n;

    // if(argc < 2) {
    //     fprintf(stderr, "Not enough arguments specified\n");
    //     fprintf(stderr, "Usage: ls [file]\n");
    //     exit(1);
    // }

    switch(*argv[1])
    {
        case '>':
            redirectLs(argv[2]);
            break;
        default:
            proc_ls(argv[1]);
            break;
    }

    exit(0);
}
