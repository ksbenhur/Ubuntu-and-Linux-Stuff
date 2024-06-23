#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void generate_processes(int n) {
    for (int i = 1; i <= n; ++i) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            printf("I am process %d\n", i);
            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            wait(NULL);  // Wait for the child to terminate
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_processes>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);

    if (n <= 0) {
        fprintf(stderr, "Please enter a positive integer as an argument.\n");
        exit(EXIT_FAILURE);
    }

    generate_processes(2 * n);

    return 0;
}