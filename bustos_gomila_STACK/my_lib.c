//PROGRAMADORES:
//RUBEN BUSTOS ROMERO
//JOAN GOMILA VIDAL

#include "lib1.h"
#include <unistd.h>

//RETURN STRING LENGTH
//INPUT: POINTER TO A STRING
//OUTPUT: STRING LEN
size_t my_strlen(const char *str){
		size_t i = 0;
		while(str[i] != '\0'){
			i++;
		}
		return i;
	}

//CONCAT. TWO STRINGS: src+dest
//INPUT: A POINTERS TO EACH STRING
//OUTPUT: RESULT OF CONCAT. ON dest
char *my_strcat(char *dest, const char *src){
		int len = my_strlen(dest);
		int i = 0;
		while(src[i]){
			dest[len + i] = src[i];
			i++;
		}
		dest[len + i] = '\0';
		return (dest);
	}

//COPY src IN dest
//INPUT: A POINTER TO EACH STRING
//OUTPUT: A COPY OF src IN dest
char *my_strcpy(char *dest, const char *src){
		char *p = dest;
		while(*src != '\0'){
			*p++ = *src++;
		}
		*p = '\0';
		return (dest);
	}

//COPY n CHARS FROM src IN dest
//INPUT: A POINTER TO EACH STRING & NUMBER OF CHARS TO COPY
//OUTPUT: A COPY OF n CHARS ON dest
char *my_strncpy(char *dest, const char *src, size_t n){
		int i = 0;

		while(n > 0 && src[i] != '\0'){
			dest[i] = src[i];
			++i;
			--n;
		}

		if(n > 0)
			dest[i] = '\0';

		return (dest);
	}

//COMPARE TWO STRINGS
//S1 < S2 -> RETURN < 0
//S1 = S2 -> RETURN 0
//S1 > S2 -> RETURN > 0
//INPUT: A POINTER TO EACH STRING
//OUTPUT: DIFF BETWEEN LENGTHS OF s1 AND s2
int my_strcmp(const char *s1, const char *s2){
		while(*s1 != '\0' && *s2 != '\0' && *s1 == *s2){
			s1++;
			s2++;
		}
		return *s1 - *s2;
	}


/////////STACK//////////

//INIT THE STACK
struct my_stack *my_stack_init(int size){
	struct my_stack *pila = malloc(sizeof(struct my_stack));
	if(pila == NULL)
		return NULL;

	pila->first = NULL;
	pila->size = size;
	return pila;
}


//ADD NODES TO STACK, EVERY NODE POINTS TO data
int my_stack_push(struct my_stack *stack, void *data){
	struct my_stack_node *nodo = malloc(sizeof(struct my_stack_node));
	if(nodo == NULL){
		return -1;
	}
	nodo->data = data;//Se asignan los datos
	//Se conectan los nodos con la pila
	nodo->next = stack->first;
	stack->first = nodo;
	return 0;
}

//RETURN DATA FROM THE TOP NODE IN STACK
void *my_stack_pop(struct my_stack *stack){
	struct my_stack_node *nodo;
	void *datos;

	nodo = stack->first;
	if(nodo == NULL)
		return NULL;

	datos = nodo->data;
	stack->first = nodo->next; //reconectamos los nodos
	free(nodo);
	return datos;
}

//AMOUNT OF NODES IN STACK
int my_stack_len(struct my_stack *stack){
	struct my_stack_node *aux0, *aux1;
	int n = 0;

	aux0 = stack->first;
	while(aux0 != NULL){
		aux1 = aux0;
		aux0 = aux1->next;
		n++;
	}
	return n;
}

//WRITE A STACK INTO A FILE
//RETURN NUMBER OF WRITTEN NODES
int my_stack_write(struct my_stack *stack, char *filename){
	struct my_stack_node *nodo = stack->first;
	int nnodos = 0;

	FILE * file = fopen(filename, "w");
	if(file == NULL){
		return nnodos;
	}

	//escribimos la informacion de la pila(size)
	fwrite(&(stack->size),sizeof(int),1,file);
	while(nodo != NULL){
		fwrite(nodo->data, stack->size, 1, file);
		//actualizamos el nodo
		nodo = nodo->next;
		nnodos++;
	}
	fclose(file);
	return nnodos;

}

//MOUNT A STACK FROM READING A FILE
//RETURN THE STACK STRUCT
struct my_stack *my_stack_read(char *filename){
	struct my_stack *stack;
	FILE *f = fopen(filename, "rb");//Creamos el fichero para leer.
	if(f!=NULL){//En el caso de que haya fichero.
		int tamStack = 0;
		int offSet = 0;
		int final = 0;
		fread(&tamStack, sizeof(int), 1, f);//Leemos el tamaño, esta en el principio del fichero.

		stack = my_stack_init(tamStack);//Inicializamos el stack.
		fseek(f, 0, SEEK_END);

		final = ftell(f);//Conseguimos el valor de la última posición del fichero.
		offSet = final-tamStack;

		int principio = sizeof(int);

		while(offSet >= principio){//Mientras el puntero sea mayor al valor donde acaba el int del principio del fichero.
			/*Leemos del fichero y hacemos un push del data*/
			void *data = malloc(tamStack);
			fseek(f, offSet, SEEK_SET);
			fread(data, tamStack, 1, f);
			my_stack_push(stack, data);
			offSet -= tamStack;
		}
		fclose(f);
	}else{
		printf("error al abrir el fichero\n");
		return NULL;
	}
	return stack;
}


//FREE STACK AND DATA
//RETURN AMOUNT OF PURGED NODES
int my_stack_purge(struct my_stack *stack){
	struct my_stack_node *aux0, *aux1;
	int liberados = 0;

	aux0 = stack->first;
	while(aux0 != NULL){
		aux1 = aux0->next;
		free(aux0->data);
		free(aux0);
		aux0 = aux1;
		liberados++;
	}
	return liberados;
}
