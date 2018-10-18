#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define true 1
#define false 0
#define bool int


int main(int argc, char *argv[]) {
    // Variable Declaration
    pid_t pid;
    char *const args[] = {"/bin/ls", "-l", NULL};

    int fd_dest = open("myls.out", (O_CREAT | O_WRONLY | O_TRUNC), (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));

    switch ((pid = fork())) {
        case -1:
            fprintf(stderr, "Fork failed");
            break;
        case 0:
            // Child process
            close(1);
            dup2(fd_dest, 1);
            execv("/bin/ls", args);
            fprintf(stderr, "Uknown command\n");
            break;
        default:
            break;
    }

    exit(0);
}
