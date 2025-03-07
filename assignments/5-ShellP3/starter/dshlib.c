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

int parse_command(char *command_str, cmd_buff_t *cmd_buff) {
    int arg_index = 0;
    bool quoteMode = false;
    char* token_start = command_str;

    for (char* ptr = command_str; *ptr != '\0'; ptr++) {
        if (*ptr == '"') {
            quoteMode = !quoteMode;
            memmove(ptr, ptr + 1, strlen(ptr));
            ptr--;
        } else if (isspace(*ptr) && !quoteMode) {
            *ptr = '\0';
            cmd_buff->argv[arg_index] = strdup(token_start);
            if (cmd_buff->argv[arg_index] == NULL) {
                return ERR_MEMORY;
            }
            arg_index++;
            if (arg_index >= CMD_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            token_start = ptr + 1;
        }
    }

    if (*token_start != '\0') {
        cmd_buff->argv[arg_index] = strdup(token_start);
        if (cmd_buff->argv[arg_index] == NULL) {
            return ERR_MEMORY;
        }
        arg_index++;
    }

    cmd_buff->argc = arg_index;
    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    if (cmd_line == NULL || *cmd_line == '\0' || is_only_whitespace(cmd_line)) {
        return WARN_NO_CMDS;
    }

    char* command_str = trim(cmd_line);
    
    if (command_str != NULL && *command_str != '\0') {
        cmd_buff->_cmd_buffer = command_str;
        int parse_rc = parse_command(command_str, cmd_buff);
        if (parse_rc != OK) {
            return parse_rc;
        }

        if (cmd_buff->argc == 0) {
            return WARN_NO_CMDS;
        }
    } else {
        return WARN_NO_CMDS;
    }

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

int exec_cmd(cmd_buff_t *cmd) {
    Built_In_Cmds exec_builtin_rc = exec_built_in_cmd(cmd);
    if (exec_builtin_rc == BI_CMD_EXIT) {
        return OK_EXIT;
    } else if (exec_builtin_rc == BI_EXECUTED) {
        return BI_EXECUTED;
    } else
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
    return OK;
}

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    if (cmd_line == NULL || *cmd_line == '\0' || is_only_whitespace(cmd_line)) {
        return WARN_NO_CMDS;
    }

    char* pipe_token = strtok(cmd_line, PIPE_STRING);
    int command_index = 0;
    
    while (pipe_token != NULL) {
        cmd_buff_t* cmd_buff = (cmd_buff_t*) malloc(sizeof(cmd_buff_t));
        memset(cmd_buff, 0, sizeof(cmd_buff_t));

        char command_s[SH_CMD_MAX];
        strcpy(command_s, pipe_token);
        char* command_str = trim(command_s);

        int build_cmd_buff_rc = build_cmd_buff(command_str, cmd_buff);
        if (build_cmd_buff_rc == ERR_CMD_OR_ARGS_TOO_BIG) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        memcpy(&clist->commands[command_index], cmd_buff, sizeof(cmd_buff_t));
        free(cmd_buff);

        command_index++;
        if (command_index >= CMD_MAX) {
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

int free_cmd_list(command_list_t *clist) {
    for (int i = 0; i < clist->num; i++) {
        cmd_buff_t cmd = clist->commands[i];
        for (int j = 0; j < cmd.argc; j++) {
            free(clist->commands[i].argv[j]);
            clist->commands[i].argv[j] = NULL;
        }
        cmd.argc = 0;
    }
    clist->num = 0;
    return OK;
}

int execute_pipeline(command_list_t *clist) {
    int pipefd[2];
    pid_t pid;
    int status;
    int in_fd = 0;
    pid_t pids[CMD_MAX];

    for (int i = 0; i < clist->num; i++) {
        if (i < clist->num - 1) {
            int pipe_rc = pipe(pipefd);
            if (pipe_rc < 0) {
                printf("Pipe error\n");
                return ERR_MEMORY;
            }
        }

        pid = fork();
        if (pid == -1) {
            printf("Fork error\n");
            return ERR_MEMORY;
        }

        if (pid == 0) {
            if (i > 0) {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }

            if (i < clist->num - 1) {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            int exec_vp_rc = execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            if (exec_vp_rc != 0) {
                exit(EXIT_FAILURE);
            }
        } else {
            pids[i] = pid;

            if (i > 0) {
                close(in_fd);
            }

            if (i < clist->num - 1) {
                close(pipefd[1]);
                in_fd = pipefd[0];
            }
        }
    }

    for (int i = 0; i < clist->num; i++) {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            printf("Unrecognized command\n");
        }
    }

    return OK;
}

void free_cmd_tools(char* cmd_buff, command_list_t* cmd_list) {
    free(cmd_buff);
    cmd_buff = NULL;

    free_cmd_list(cmd_list);
    free(cmd_list);
    cmd_list = NULL;
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
    char *cmd_buff;
    int rc = 0;
    command_list_t* cmd_list;

    while (1)
    {   
        cmd_buff = (char*) malloc(SH_CMD_MAX);
        memset(cmd_buff, 0, sizeof(char));

        cmd_list = (command_list_t*) malloc(sizeof(command_list_t));
        memset(cmd_list, 0, sizeof(command_list_t));

        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }
        // remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';
        
        rc = build_cmd_list(cmd_buff, cmd_list);
        
        if (rc == ERR_CMD_OR_ARGS_TOO_BIG) {
            printf("Error: Command or args too large\n");
        } else if (rc == WARN_NO_CMDS) {
            printf(CMD_WARN_NO_CMD);
        } else if (rc == ERR_TOO_MANY_COMMANDS) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
        } else {
            Built_In_Cmds exec_builtin_rc = exec_built_in_cmd(&cmd_list->commands[0]);
            if (exec_builtin_rc == BI_CMD_EXIT) {
                free_cmd_tools(cmd_buff, cmd_list);
                break;
            } else if (exec_builtin_rc == BI_EXECUTED || exec_builtin_rc == BI_RC) {
                free_cmd_tools(cmd_buff, cmd_list);
                continue;
            }

            int exec_pipeline_rc = execute_pipeline(cmd_list);
            if (exec_pipeline_rc == OK_EXIT) {
                free_cmd_tools(cmd_buff, cmd_list);
                break;
            }
        }

        free_cmd_tools(cmd_buff, cmd_list);
    }

    return OK;
}
