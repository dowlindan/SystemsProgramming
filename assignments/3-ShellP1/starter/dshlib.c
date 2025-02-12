#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

char* trim(char *str)
{
    char *end;

    while(isspace(*str)) {
        str++;
    }

    end = str + strlen(str) - 1;

    while(end > str && isspace(*end)) {
        end--;
    }

    end[1] = '\0';

    return str;
}
/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    char* pipe_token = strtok(cmd_line, PIPE_STRING);
    int command_index = 0;
    
    while (pipe_token != NULL) {
        command_t* command = (command_t*) malloc(sizeof(command_t));

        char command_s[SH_CMD_MAX];
        strcpy(command_s, pipe_token);
        char* command_str = trim(command_s);

        if (command_str != NULL) {
            char exe[EXE_MAX];
            char args[ARG_MAX];
  
            char* space_pos = strchr(command_str, SPACE_CHAR);
            if (space_pos != NULL) {
                int exe_length = space_pos - command_str;

                if (exe_length > EXE_MAX) {
                    return ERR_CMD_OR_ARGS_TOO_BIG;
                }
                strncpy(exe, command_str, exe_length);
                exe[exe_length] = '\0';

                char* args_parsed = space_pos + 1;
                if (strlen(args_parsed) > ARG_MAX) {
                    return ERR_CMD_OR_ARGS_TOO_BIG;
                }
                strcpy(args, args_parsed);
            } else {
                strcpy(exe, command_str);
                strcpy(args, "");
            }
            strcpy(command->exe, exe);
            strcpy(command->args, args);
        }

        clist->commands[command_index] = *command;
        command_index++;
        if (command_index > CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }
        pipe_token = strtok(NULL, PIPE_STRING);
    }

    clist->num = command_index;

    if (command_index == 0) {
        return WARN_NO_CMDS;
    }

    return OK;
}