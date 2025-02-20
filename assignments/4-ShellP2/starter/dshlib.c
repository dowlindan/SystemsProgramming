#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

bool is_only_whitespace(char* str) {
    char* ptr = str;
    while (*ptr!='\0') {
        if (!isspace(*ptr)) {
            return false;
        }
        ptr++;
    }
    return true;
}

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

void replace_spaces_with_null_terminators(char* str) {
    char* ptr = str;
    bool quoteMode = false;

    while (*ptr != '\0') {
        if (*ptr == '"') {
            quoteMode = !quoteMode;
            memmove(ptr, ptr+1, strlen(ptr));
        } else if (isspace(*ptr) && !quoteMode) {
            *ptr = '\0';
        }
        ptr++;
    }
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    if (cmd_line == NULL || *cmd_line == '\0' || is_only_whitespace(cmd_line)) {
        return WARN_NO_CMDS;
    }

    char* command_str = trim(cmd_line);
    
    if (command_str != NULL && *cmd_line != '\0') {
        replace_spaces_with_null_terminators(command_str);
        
        cmd_buff->_cmd_buffer = command_str;
        
        int arg_index = 0;

        char* ptr = command_str;
        while (*ptr != '\0') {
            cmd_buff->argv[arg_index] = (char*) malloc(strlen(ptr) + 1);
            
            if (*ptr == '"') {
                ptr++;
            }
            ptr[strcspn(ptr, "\"")] = '\0';

            strcpy(cmd_buff->argv[arg_index], ptr);

            arg_index++;
            if (arg_index > CMD_ARGV_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            ptr += strlen(ptr) + 1;  // Next token
        }

        cmd_buff->argc = arg_index;

    } else {
        return WARN_NO_CMDS;
    }

    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->argc = 0;
    cmd_buff->_cmd_buffer = NULL;
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    } 
    return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
    for (int i = 0; i < cmd_buff->argc; i++) {
        free(cmd_buff->argv[i]);
        cmd_buff->argv[i] = NULL;
    }
    free(cmd_buff->_cmd_buffer);
    cmd_buff->_cmd_buffer = NULL;
    cmd_buff->argc = 0;
    return OK;
}

Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, EXIT_CMD) == 0) {
        return BI_CMD_EXIT;
    }  else if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
    } else {
        return BI_NOT_BI;
    }
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    Built_In_Cmds cmd_type = match_command(cmd->argv[0]);
    switch (cmd_type) {
        case BI_CMD_EXIT:
            return BI_CMD_EXIT;
        case BI_CMD_CD:
            if ((cmd->argc == 2 && chdir(cmd->argv[1]) == 0) || cmd->argc == 1)  {
                return BI_EXECUTED;
            } else {
                printf("Improper cd usage\n");
                return BI_RC;
            }
        default:
            return BI_NOT_BI;
    }
}

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char *cmd_buff = (char*) malloc(ARG_MAX);
    int rc = 0;
    cmd_buff_t* cmd = (cmd_buff_t*) malloc(sizeof(cmd_buff_t));

    while (1)
    {   
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }
        // remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        rc = build_cmd_buff(cmd_buff, cmd);
        
        if (rc == ERR_CMD_OR_ARGS_TOO_BIG) {
            printf("Error: Command or args too large\n");
        } else if (rc == WARN_NO_CMDS) {
            printf(CMD_WARN_NO_CMD);
        } else {
            Built_In_Cmds exec_builtin_rc = exec_built_in_cmd(cmd);
            if (exec_builtin_rc == BI_CMD_EXIT) {
                return OK;
            }
            if (exec_builtin_rc == BI_NOT_BI) {
                int fork_rc, exec_rc;
                fork_rc = fork();
                if (fork_rc < 0) {
                    printf("Fork Error\n");
                }
                else if (fork_rc == 0) {
                    int execvp_rc = execvp(cmd->argv[0], cmd->argv);
                    if (execvp_rc < 0) {
                        printf("Unrecognized command\n");
                        exit(1);
                    }
                } else {
                    wait(&exec_rc);
                }
            }
            clear_cmd_buff(cmd);
        }

    }

    free_cmd_buff(cmd);

    return OK;
}
