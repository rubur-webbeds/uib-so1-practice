#include "minishell.h"

char *params[PARAM_SIZE]; //params[0] = instr, params[1] = params, params[2] = null
char sep[7] = " #=\r\n\t";
char *commands[] = {"cd", "export", "source", "jobs", "exit", "NULL"};
static struct info_process process_list[N_JOBS];
static int n_pids; //number of executing processes

int main(){

  signal(SIGINT, ctrlc);
  signal(SIGCHLD, reaper);
  signal(SIGTSTP, ctrlz);

  char *line = malloc(COMMAND_LINE_SIZE);
  if(line == NULL){
    perror("main() ERROR");
    return -1;
  }


  while(read_line(line)){
    execute_line(line);
  }
  free(line);

  return 0;
}

/*
Prints a custom prompt
The prompt consists of the CWD concat "$"
*/
void print_prompt(){
  char *prompt = malloc(100);
  getcwd(prompt, 100);

  strcat(prompt, "$ ");
  printf("%s", prompt);
  free(prompt);
}

/*
Reads a line from stdinput(keyboard) and prints the prompt
INPUT PARAM: ptr to a buffer where to store the input
OUTPUT PARAM: ptr to the read line
*/
char *read_line(char *line){
  print_prompt();
  char *ptr;

  ptr = fgets(line, COMMAND_LINE_SIZE, stdin);


  if (!ptr) {
    if (feof(stdin)){
      /////////( TDOO
      return NULL;
    }
    line[0] = 0;
  }
  return line;
}

/*
Executes a line
INPUT PARAM: ptr to the line to be executed
OUTPUT PARAM: 0 = OK, otherwise -1
*/
int execute_line(char *line){
  if (strlen(line) == 0) {
    return 0;
  }

  if(parse_args(params, line) < 0){
    printf("ERROR: parse_args()\n");
    return -1;
  }
  int is_back = is_background(params);
  int chck = check_internal(params);
  if(chck == -1){
    external_command(params, line);
  }

  return 0;
}

/*
Parses a line into tokens
INPUT PARAM: args as a ptr to the parsed params and line as a ptr to read from
OUTPUT PARAM: number of parsed params
*/
int parse_args(char **args, char *line){
	int i = 0;

	//First param
	args[0] = strtok(line, sep);

	while(args[i] != NULL){
		i++;
		args[i] = strtok(NULL, sep);
	}
	return i;
}

/*
Checks if a command is in our implemented command array
if it doesn't match means it's an external command, which is tagged as "NULL"
INPUT PARAM: args[0] is the command to check
OUTPUT PARAM: the position in the commands array
*/
int check_internal(char **args){
  int cmp, i = 0;

	cmp = strcmp(args[0], commands[i]);
	while(cmp != 0){
    if(strcmp(commands[i], "NULL") == 0){
      return -1;
    }
		i++;
		cmp = strcmp(args[0], commands[i]);
	}

  switch (i) {
    case 0:
      internal_cd(params);
      break;
    case 1:
      internal_export(params);
      break;
    case 2:
      internal_source(params);
      break;
    case 3:
      internal_jobs(params);
      break;
    case 4:
      exit(0);
    }

    return 0;
}

/*
Changes the working directory to the one specified
if no directory is specified, then change directory to HOME
INPUT PARAM: args[1] is the new directory
OUTPUT PARAM: 0 -> OK, -1 -> ERROR
*/
int internal_cd(char **args){
  char *path = args[1];
  char *dir = malloc(COMMAND_LINE_SIZE);

  //checking paramaters
  if(args[2] != NULL){
    printf("ERROR: INCORRECT SYNTAX.\n");
    printf("USAGE: $ cd [path]\n");
    return  -1;
  }

  //getting current working directory
  if(getcwd(dir, COMMAND_LINE_SIZE) == NULL){
    perror("getcwd() ERROR");
    return -1;
  }
  printf("Current Working Directory: %s\n", dir);

  // $ cd null => cd HOME
  if(path == NULL){
    char *home;
    home = getenv("HOME");
    if(chdir(home) == -1){
      perror("chdir(HOME) ERROR");
      return -1;
    }
    return 0;
  }

  //change working directory
  if(chdir(path) == -1){
    perror("chdir(path) ERROR");
    return -1;
  }

  //getting current working directory after changing it
  if(getcwd(dir, COMMAND_LINE_SIZE) == NULL){
    perror("getcwd() ERROR");
    return -1;
  }
  printf("New Working Directory: %s\n", dir);

  free(dir);

  return 0;
}
int internal_export(char **args){
  char *name = args[1];
  char *value = args[2];

  //checking paramaters
  if(args[1] == NULL || args[2] == NULL || args[3] != NULL){
    printf("ERROR: INCORRECT SYNTAX.\n");
    printf("USAGE: $ export [NAME]=[VALUE]\n");
    return  -1;
  }

  //get current value of NAME
  char *current_value = getenv(name);
  if(current_value == NULL){
    perror("getenv() ERROR");
    return -1;
  }
  printf("Current Value: %s\n", current_value);

  //set new value to NAME
  //int setenv(const char *envname, const char *envval, int overwrite);
  if(setenv(name, value, 1) == -1){
    perror("set env() ERROR");
    return -1;
  }

  //get new current value
  current_value = getenv(name);
  if(current_value == NULL){
    perror("getenv() ERROR");
    return -1;
  }
  printf("Current Value: %s\n", current_value);

  return 0;
}

/*
reads a file and executes every command in it
INPUT PARAM: args[1] as the name of the script
OUTPUT PARAM: 0 -> OK, -1 -> ERROR
*/
int internal_source(char **args){
  char *filename = args[1];
  char line[COMMAND_LINE_SIZE];

  //checking paramaters
  if(args[1] == NULL || args[2] != NULL){
    printf("ERROR: INCORRECT SYNTAX.\n");
    printf("USAGE: $ source [filename]\n");
    return  -1;
  }

  //opening the file to execute
  FILE *file = fopen(filename, "r");
  if(file == NULL){
    perror("fopen() ERROR");
    return -1;
  }

  //reading and executing each line
  while(fgets(line, COMMAND_LINE_SIZE, file) != NULL){
    execute_line(line);
  }
  fclose(file);

  return 0;
}
int internal_jobs(char **args){
  printf("Do shit in jobs\n");
  return 0;
}

/*
executes a foreign command using fork() to create another process
and execvp() to execute the command
INPUT PARAM: args as the parsed command line
OUTPUT PARAM: 0 -> OK, -1 -> ERROR
*/
int external_command(char **args, char *line){
  char *command = args[0];
  pid_t pid;

  pid = fork();
  if(pid == 0){ //son
    signal(SIGINT, SIG_DFL);
    signal(SIGCHLD, SIG_DFL);

    execvp(command, args); //execvp doesn't return anything
    //code here only executes if execvp fails
    perror("execvp() ERROR");
    exit(-1);
  }else if(pid > 0){ //father
    process_list[0].pid = pid; //the foreground process PID is 'pid'
    //wait(NULL);
    while(process_list[0].pid != 0){
      pause();
    }
  }else{ //error
    perror("fork() ERROR");
    return -1;
  }
  return 0;
}

/*
checks if the any of the args pased as parameter contains the '&' symbol
INPUT PARAM: args as the parsed command line
OUTPUT PARAM: 0 -> contains '&', -1 -> don't contains '&'
*/
int is_background(char **args){
  int i = 0;
  while(args[i] !=  NULL){
    if(strcmp(args[i], "&") == 0){
      args[i] = NULL;
      return 0;
    }
    i++;
  }
  return -1;
}

//SIGCHLD handler
void reaper(int signum){
  signal(SIGCHLD, reaper);

  pid_t dead = waitpid(-1, NULL, WNOHANG);
  if(dead == process_list[0].pid){ //if the foreground process has died...
    //printf("+ FINISHED PROCESS [%d]\n", dead);
    process_list[0].pid = 0;
  }
}

//SIGINT handler
void ctrlc(int signum){
  signal(SIGINT, ctrlc);

  if(process_list[0].pid > 0){ //if the foreground process doesn't finished yet...
    kill(process_list[0].pid, SIGINT);
  }

  printf("I'M NOT THE FG PROCESS, I WONT DIE: %d\n", getpid());
}

//SIGSTP handler
void ctrlz(int signum){
  signal(SIGTSTP, ctrlz);
  //add the process to process_list[n], n > 0 (0 is just for the fg process)
  //change the status of the process from 'X' to 'S'
}
