#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

size_t my_strlen(const char *);
char  *my_strcat(char *dest, const char *src);
char  *my_strcpy(char *dest, const char *src);
char  *my_strncpy(char *dest, const char *src, size_t n);
int    my_strcmp(const char *s1, const char *s2);


struct my_stack_node {
	void *data;
	struct my_stack_node *next;
};

struct my_stack {
	int size;
	struct my_stack_node *first;
};

struct my_stack *my_stack_init(int size);
int my_stack_push(struct my_stack *stack, void *data);
void *my_stack_pop(struct my_stack *stack);
int  my_stack_len(struct my_stack *stack);
int my_stack_write(struct my_stack *stack, char *filename);
struct my_stack *my_stack_read(char *filename);
int my_stack_purge(struct my_stack *stack);
