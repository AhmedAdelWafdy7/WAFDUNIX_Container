/**
 * @file container.c
 * @brief This file contains the implementation of a container creation program.
 */

#define _GNU_SOURCE

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mount.h>


#define STACK_SIZE (1024 * 1024)
static char child_stack[STACK_SIZE];


int create_container(void);
int child(void *arg);


/**
 * @brief The main function of the program.
 * 
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return int The exit status of the program.
 */
int main() {
    int namespace_flags = CLONE_NEWPID | CLONE_NEWNS | CLONE_NEWNET | CLONE_NEWUTS | CLONE_NEWIPC | SIGCHLD;
    pid_t container = clone(child, child_stack + STACK_SIZE, namespace_flags, NULL);
    if (container == -1) {
        perror("clone");
        exit(EXIT_FAILURE);
    }
    waitpid(container, NULL, 0);
    return 0;
}

/**
 * @brief Function to create a container.
 * 
 * This function creates a container by performing the following steps:
 * - Change the root directory to "/home/rootfs" using chroot.
 * - Change the current working directory to "/" using chdir.
 * - Mount the proc filesystem at "/proc" using mount.
 * - Execute the "/bin/bash" command using execv.
 * 
 * @return int The exit status of the function.
 */
int create_container(void) {
    char *cmd[] = {"/bin/bash", NULL};

    chroot("/home/rootfs");
    chdir("/");
    mount("proc", "/proc", "proc", 0, "");
    execv(cmd[0], cmd);
    perror("execv");
    exit(EXIT_FAILURE);
}

/**
 * @brief Function to be executed by the child process.
 * 
 * This function is passed as an argument to the clone function and is executed by the child process.
 * It calls the create_container function to create a container.
 * 
 * @param arg The argument passed to the child function.
 * @return int The exit status of the function.
 */
int child(void *arg) {
    (void)arg; // Correct way to suppress the warning about the unused parameter
    create_container();
    return 0;
}