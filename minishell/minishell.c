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
      return i;
    }
		i++;
		cmp = strcmp(args[0], commands[i]);
	}

	return i;
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
  if(getcwd(dir, COMMAND_LINE_SIZE) == -1){
    perror("ERROR:");
    return -1;
  }
  printf("Current Working Directory: %s\n", dir);

  // $ cd null => cd HOME
  if(path == NULL){
    char *home;
    home = getenv("HOME");
    if(chdir(home) == -1){
      perror("ERROR:");
      return -1;
    }
    return 0;
  }

  //checking if the directory contains any blank space
  if(check_symbol(path, ' ') == 0){
    //add quotes to the path so chdir() can succeed
    add_pre_suf(path, '"', '"');
    //change working directory
    if(chdir(path) == -1){
      perror("ERROR:");
      return -1;
    }
  }

  //change working directory
  if(chdir(path) == -1){
    perror("ERROR:");
    return -1;
  }

  //getting current working directory after changing it
  if(getcwd(dir, COMMAND_LINE_SIZE) == -1){
    perror("ERROR:");
    return -1;
  }
  printf("New Working Directory: %s\n", dir);

  free(dir);

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

/*
Checks if str contains any symbol
INPUT PARAM: str is the string to check
OUTPUT PARAM: 0 -> str has at least one symbol, -1 -> str does not have any symbol
*/
int check_symbol(char *str, char symbol){
  while(str){
    if(str == symbol){
      printf("LO TIENE\n");
      return 0;
    }
  }
  return -1;
}

/*
Concts prefix at the beginning of str and suffix at the end
INPUT PARAM: str is the string to modify
             prefix is the char to concat at the beginning
             suffix is the char to concat at the end
OUTPUT PARAM: 0 -> OK, -1 -> ERROR
*/
int add_pre_suf(char *str, char prefix, char suffix){
  return 0;
}
