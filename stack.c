#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// header contents start--------------------------------------------------------
// #include<stack.h>
#include <stdbool.h>

#ifndef BABYC_STACK_HEADER
#define BABYC_STACK_HEADER

typedef struct Stack {
    int size;
    void **content;
} Stack;

Stack *stack_new();

void stack_free(Stack *stack);
void stack_push(Stack *stack, void *item);
void *stack_pop(Stack *stack);
void *stack_peek(Stack *stack);
bool stack_empty(Stack *stack);

#endif
// header contents end----------------------------------------------------------

Stack *stack_new() {
    Stack *stack = malloc(sizeof(Stack));
    stack->size = 0;
    stack->content = 0;

    return stack;
}

void stack_free(Stack *stack) {
    if (stack->size > 0) {
        free(stack->content);
    }
    free(stack);
}

void stack_push(Stack *stack, void *item) {
    stack->size++;

    // We expand the memory allocated by one word, then write the new
    // value to the end.
    stack->content =
        realloc(stack->content, stack->size * sizeof *stack->content);
    stack->content[stack->size - 1] = item;
}

void *stack_pop(Stack *stack) {
    assert(stack->size >= 1);
    stack->size--;

    void *item = stack->content[stack->size];
    stack->content =
        realloc(stack->content, stack->size * sizeof *stack->content);
    return item;
}

void *stack_peek(Stack *stack) {
    assert(stack->size >= 1);

    return stack->content[stack->size - 1];
}

bool stack_empty(Stack *stack) { return stack->size == 0; }
