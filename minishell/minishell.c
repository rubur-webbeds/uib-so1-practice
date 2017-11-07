#include "minishell.h"

int main(){
  while(read_line()){
    execute_line();
  }

  return 0;
}


char *read_line(char *line){

}
