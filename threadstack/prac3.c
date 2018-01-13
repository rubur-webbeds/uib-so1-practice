#include "lib1.h"
#include "prac3.h"

pthread_t thread_id[N_THREADS];//array de id threads
pthread_mutex_t mutex;
struct my_stack *stack = NULL;

int main(int argc, char **argv){

	int len = 0; //longitud de la pila
	//struct my_stack *pila_prueba = NULL;
	
	struct my_data *data = NULL;
	pthread_mutex_init(&mutex, NULL);
	
	int i;

	if(argc != 2){
		printf("ERROR: argumentos incorrectos!\n");
		printf("USO: $./prac3 [nombre_pila]");
		return -1;
	}

	char *nom_pila = argv[1];
	//creamos pila
	//pila_prueba = my_stack_init(sizeof(struct my_data));
	//escribimos pila en file
	//my_stack_write(pila_prueba, "stack.txt");

	//leemos la pila y la montamos en stack
	stack = my_stack_read(nom_pila);
	if(stack == NULL){
		stack = my_stack_init(sizeof(struct my_data));
	}

	len = my_stack_len(stack);

	//rellenamos si el numero de elementos es < 10
	while(len < MAX){
		data = malloc(sizeof(struct my_data));
		data->val = 0;
		my_stack_push(stack, data);
		len = my_stack_len(stack);
		//len++;
	}
	//free(data);

	for(i = 0; i < N_THREADS; i++){
		if(pthread_create(&thread_id[i], NULL, coge_incrementa_deja ,NULL) != 0){
			//Error
			printf("ERROR: pthread_create().\n");
			return -1;
		}
		printf("Thread started.\n");
	}

	for(i = 0; i < N_THREADS; i++){
		if(pthread_join(thread_id[i], NULL) != 0){
			//Error
			printf("ERROR: pthread_join().\n");
			return -1;
		}
	}

	//escribimos la pila en el fichero
	my_stack_write(stack, nom_pila);
	printf("Bye from main\n");
	pthread_exit(NULL);

	return 0;
}

void *coge_incrementa_deja(){
	struct my_data *data = NULL;
	//struct my_stack *stack = NULL;
	long value;

	for(long i = 0; i < T_MAX; i++){
		//seccion critica
		pthread_mutex_lock(&mutex);
		data = my_stack_pop(stack);
		pthread_mutex_unlock(&mutex);
		//incremento del valor de los datos
		value = data->val;
		value++;
		data->val = value;
		//seccion critica
		pthread_mutex_lock(&mutex);
		my_stack_push(stack, data);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}






