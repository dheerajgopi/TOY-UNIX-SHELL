#include "header.h"
#include "piping.c" /* contains function for piping */
#include "parse.c"

void execute(char *cmd_array[][MAX_ARGS], int cmd_no);

int main()
{
    char input[100];
    char *cmd_array[MAX_CMDS][MAX_ARGS];
    char *newline;
    int cmd_no;

    while (1) {
        printf(">");
        fgets(input, sizeof(input), stdin);

        /* string termination. change \n to \0 */
        newline = strchr(input, '\n');
        if (newline)
            *newline = 0;

        /* parse the input and create a 2D array of commands 
         * for execution. Also counts the num of commands */
        parse(input, cmd_array, &cmd_no);

        /* execute the parsed commands */
        execute(cmd_array, cmd_no);
    }
}

void execute(char *cmd_array[][MAX_ARGS], int cmd_no)
{
    int pid;

    pid = fork();

    switch(pid) {
    case -1:
        fprintf(stderr, "fork failed!\n");
        break;

    case 0:     /* child process */
        if (cmd_no == 1)
            execvp(cmd_array[0][0], cmd_array[0]);
        else
            piping(cmd_array, cmd_no);
        break;

    default:    /* parent process */
        wait(&pid);
        break;
    }
}
