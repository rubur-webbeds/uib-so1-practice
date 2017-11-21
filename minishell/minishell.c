#include "minishell.h"

char *params[PARAM_SIZE]; //params[0] = instr, params[1] = params, params[2] = null
char sep[6] = " #\r\n\t";
char *commands[] = {"cd", "export", "source", "jobs", "exit", "NULL"};


int main(){
  char *line = malloc(COMMAND_LINE_SIZE);
  if(line == NULL){
    perror("ERROR");
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

  strcat(prompt, "$");
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
  fgets(line, COMMAND_LINE_SIZE, stdin);
  return line;
}

/*
Executes a line
INPUT PARAM: ptr to the line to be executed
OUTPUT PARAM: 0 = OK, otherwise -1
*/
int execute_line(char *line){
  if(parse_args(params, line) < 0){
    perror("ERROR");
    return -1;
  }
  int chck = check_internal(params);
  switch (chck) {
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
      break;
    default:
      external_command(params);
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
Parses a line into tokens
INPUT PARAM: args as a ptr to the parsed params and line as a ptr to read from
OUTPUT PARAM:
*/
int check_internal(char **args){
  int cmp, i = 0;

	cmp = strcmp(args[0], commands[i]);
	while(cmp != 0){
    if(strcmp(commands[i], "NULL") == 0){
      return i;
    }
		i++;
		cmp = strcmp(args[0], commands[i]);
	}

	return i;
}

int internal_cd(char **args){
  printf("Do shit in cd\n");
  return 0;
}
int internal_export(char **args){
  printf("Do shit in export\n");
  return 0;
}
int internal_source(char **args){
  printf("Do shit in source\n");
  return 0;
}
int internal_jobs(char **args){
  printf("Do shit in jobs\n");
  return 0;
}

int external_command(char **args){
  printf("EXTERNAL\n");
  return 0;
}
