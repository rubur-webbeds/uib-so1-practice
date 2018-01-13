# include "lib1.h"
# include "prac3.h"


int main(int argc, char **argv){

	struct my_stack *stack;
	struct my_data *data;
	int i , items;
	long num, min = 0, sum = 0, max = 0, avg = 0;

	stack = my_stack_read(argv[1]);
	if(stack == NULL){
		printf("ERROR: No existe la pila.' %s.\n",argv[1]);
		exit(1);
	}

	items = my_stack_len(stack);

	printf("Stack lenght: %d\n", items);
	
	for(i = 0; i < items; i++){
		//Reads item and saves the value in num 
		data = my_stack_pop(stack);
		num = data->val;
		printf("%ld\n", num);
		sum = sum + num;
		if(i == 0){
			min = num;
		}
		else if (num < min){
			min = num;
		}
		if(num > max){
			max = num;
		}
	}

	if(items != 0){
		avg = sum/items;
	}
	printf("Items: %d Sum: %ld Min: %ld Max: %ld Average: %ld \n",items,sum,min,max,avg);
	return 0;
}
