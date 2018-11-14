#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int ls(int pipe[]) {
    char *const args[] = {"/bin/ls", "-l", NULL};
    close(1);

    dup2(pipe[1], 1);

    close(pipe[0]);
    close(pipe[1]);

    int res = execv("/bin/ls", args);
    if (res == -1) fprintf(stderr, "LS Failed\n");
}

int grep(int pipe1[], int pipe2[]) {
    char *const args[] = {"/bin/grep", "main.c", NULL};
    close(0);
    close(1);

    // Read from pipe[0] = 3, which is where ls is writing to
    dup2(pipe1[0], 0);
    // close(pipe1[0]);
    close(pipe1[1]);

    // Write to pipe2[1] = 6, which is the write end from where wc will read
    dup2(pipe2[1], 1);
    // close(pipe2[0]);
    // close(pipe2[1]);
    int res = execv("/bin/grep", args);
    if (res == -1) fprintf(stderr, "GREP Failed\n");
}

int wc(int pipe1[], int pipe2[]) {
    char *const args[] = {"/usr/bin/wc", NULL};
    close(0);
    close(pipe1[0]);
    close(pipe1[1]);

    dup2(pipe2[0], 0);

    close(pipe2[0]);
    close(pipe2[1]);

    int res = execv("/usr/bin/wc", args);
    if (res == -1) fprintf(stderr, "WC Failed\n");
}

int main(void) {
    /* Insert your code here */
    pid_t pid;
    pid_t pid2;
    int pipe1[2], pipe2[2];

    pipe(pipe1);
    switch ((pid = fork())) {
        case -1:
            fprintf(stderr, "Failed to work in main\n");
            break;
        case 0:
            pipe(pipe2);
            switch ((pid2 = fork())) {
                case -1:
                    fprintf(stderr, "Grep child failed to fork\n");
                    break;
                case 0:
                    grep(pipe1, pipe2);
                    break;
                default:
                    wc(pipe1, pipe2);
                    break;
            }
            break;
        default:
            ls(pipe1);
            break;
    }

    exit(0);
}
