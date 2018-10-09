#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define CP_STD_IN 0
#define CP_STD_OUT 1
#define CP_STD_ERR 2

/**
 * File Descriptor Values:
 *  0 - Standard input
 *  1 - Standard output
 *  2 - Standard error
 */

int main(int argc, char * argv[]) {

    // Variable Declaration
    char buf[512];
    double n;

    if(argc < 3) {
        fprintf(stderr, "Not enough arguments specified\n");
        fprintf(stderr, "Usage: cp file1 file2\n");
        exit(1);
    }

    // Attempt to open the file and exit if it fails
    printf("Copying: %s to file: %s\n", argv[1], argv[2]);
    int fd_source = open(argv[1], O_RDONLY);
    int fd_dest = open(argv[2], (O_CREAT | O_WRONLY | O_TRUNC), (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));

    if(fd_source < 0) 
    {
        fprintf(stderr, "Could not open file %s\n", argv[1]);
        exit(1);
    }
    for (;;) {
        n = read(fd_source, buf, sizeof buf);
        if (n == 0)
            break;
        if (n < 0) {
            fprintf(stderr, "read error\n");
            exit(1);
        }
        if (write(fd_dest, buf, n) != n) {
            fprintf(stderr, "write error\n");
            exit(1);
        }
    }
    close(fd_source);
    close(fd_dest);

    exit(0);
}

