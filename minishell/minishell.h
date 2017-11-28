#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define COMMAND_LINE_SIZE 1024
#define PARAM_SIZE 3
#define N_JOBS 10

//MINISHELL FUNCTIONS
void print_prompt();
char *read_line(char *line);
int execute_line(char *line);
int parse_args(char **args, char *line);
int check_internal(char **args);
int internal_cd(char **args);
int internal_export(char **args);
int internal_source(char **args);
int internal_jobs(char **args);
int external_command(char **args, char *line);
void reaper(int signum);

//PROCESS STRUCT
struct info_process {
    pid_t pid;
    char status; // ’X’ of executing or ‘S’ of stopped
    char command_line[COMMAND_LINE_SIZE];
};
