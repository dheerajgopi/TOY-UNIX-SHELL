#include "header.h"

void output_redirect(char *input_arg_array[], int output_file_index)
{
    int fd, pid;
    char *buf;

    /* extract the filename from input argument array */
    buf = input_arg_array [output_file_index];

    /* remove the '>' from input argument array */
    input_arg_array[output_file_index-1] = 0;

    pid = fork();

    switch (pid) {
    case -1:
        fprintf(stderr, "fork failed!\n");
        break;

    case 0:
        fd = open(buf, O_RDWR | O_CREAT | O_TRUNC, 0644);
        close(1);
        dup(fd);
        execvp(input_arg_array[0], input_arg_array);

    default:
        wait(&pid);
    }
}

/* Handles piping operations in the input command.
 * NOTE: 2 pipe file descriptors are used to share data between the
 * commands. pfd1 for odd iterations and pfd2 for even iterations. */
void piping(char *cmd_array[][MAX_ARGS], int cmd_no)
{
    int pfd1[2], pfd2[2];   /* file descriptor for pipes */

    int i, pid;

    for (i=0; i<cmd_no; i++) {
        
        if (i % 2 != 0)
            pipe(pfd1);     /* if i is odd */
        else
            pipe(pfd2);     /* if i is even */

        pid = fork();
        switch (pid) {
        case -1:
            /* close file descriptors if fork fails */
            if (i != cmd_no - 1) {
                if (i % 2 != 0)
                    close(pfd1[1]);
                else
                    close(pfd2[1]);
            }

            fprintf(stderr, "fork failed!\n");
            return;

        case 0:
            /* if first command */
            if (i == 0)
                dup2(pfd2[1], STDOUT_FILENO);

            /* if last command */
            else if (i == cmd_no - 1) {
                if (cmd_no % 2 != 0)
                    dup2(pfd1[0], STDIN_FILENO);
                else
                    dup2(pfd2[0], STDIN_FILENO);
            }

            /* if command in between */
            else {
                if (i % 2 != 0) {
                    dup2(pfd2[0], STDIN_FILENO);
                    dup2(pfd1[1], STDOUT_FILENO);
                }
                else {
                    dup2(pfd1[0], STDIN_FILENO);
                    dup2(pfd2[1], STDOUT_FILENO);
                }
            }

            /* exec each command */
            if (execvp(cmd_array[i][0], cmd_array[i]) == -1) {
                fprintf(stderr, "exec failed!\n");
                return;
            }
        }

        /* closing pipe file descriptors */
        if (i == 0)
            close(pfd2[1]);

        else if (i == cmd_no - 1) {
            if (cmd_no % 2 != 0)
                close(pfd1[0]);
            else
                close(pfd2[0]);
        }

        else {
            if (i % 2 != 0) {
                close(pfd2[0]);
                close(pfd1[1]);
            }
            else {
                close(pfd1[0]);
                close(pfd2[1]);
            }
        }

        wait(&pid);
    }
}
