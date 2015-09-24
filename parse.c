/* 'parse' function parses the shell user input (char input[]) and 
 * fills each row of the 2D array (char *cmd_array) with a command in
 * the pipeline. Each column in the row represents the arguments of the
 * respective command. 'argument_nos' represents the number of commands
 * piped. */

#include "header.h"

void parse(char input[100], char *cmd_array[][MAX_ARGS], int *command_nos)
{
    char *cmd_arg;
    int cmd_no = 0;     /* represents no of commands */
    int arg_c = 0;      /* represents no of args in a command */

    /* tokenize input, delimited by " " */
    cmd_arg = strtok(input, " ");

    /* walk through tokens, create 2D array (cmd_array[cmd][args])*/
    while (cmd_arg != NULL) {

        /* check for pipe. If true move to next row */
        if (strcmp(cmd_arg, "|") == 0) {
            cmd_array[cmd_no][arg_c] = 0;
            arg_c = 0;
            cmd_no++;
        }

        /* check for output_redirection */
        else if (strcmp(cmd_arg, ">") == 0) {
        }

        /* else create command array for execvp */
        else {
            cmd_array[cmd_no][arg_c] = cmd_arg;
            arg_c++;
        }

        cmd_arg = strtok(NULL, " ");
    }

    /* NULL termination and updating no of commands */
    cmd_array[cmd_no][arg_c] = 0;
    cmd_no++;
    *command_nos = cmd_no;
}
