#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "tested_declarations.h"
#include "rdebug.h"

int stack_init(struct stack_t **stack, int N){
    if(stack==NULL||N<1)
        return 1;

    *stack=malloc(sizeof(struct stack_t));
    if(*stack==NULL){
        *stack=NULL;
        return 2;
    }

    (*stack)->data=malloc(N*sizeof(int));
    if((*stack)->data==NULL){
        free(*stack);
        *stack=NULL;
        return 2;
    }

    (*stack)->capacity=N;
    (*stack)->head=0;
    return 0;
}

int stack_push(struct stack_t *stack, int value){
    if(stack==NULL||stack->data==NULL||stack->capacity<1||stack->head<0||stack->head>stack->capacity)
        return 1;

    if(stack->head==stack->capacity){
        int *nowy_stos=realloc(stack->data,2*stack->capacity*sizeof(int));
        if(nowy_stos==NULL)
            return 2;

        stack->data=nowy_stos;
        stack->capacity=stack->capacity*2;
    }

    *(stack->data+stack->head++)=value;
    return 0;
}

int stack_pop(struct stack_t *stack, int *err_code){
    if(err_code!=NULL){
        if(stack==NULL||stack->data==NULL||stack->capacity<1||stack->head<0||stack->head>stack->capacity){
            *err_code=1;
            return 1;
        }

        if(stack->head==0){
            *err_code=2;
            return 2;
        }

        *err_code=0;
        return *(stack->data+(--stack->head));
    }
    else{
        if(stack==NULL||stack->data==NULL||stack->capacity<1||stack->head<0||stack->head>stack->capacity)
            return 1;

        if(stack->head==0)
            return 2;

        return *(stack->data+(--stack->head));
    }
}

void stack_display(const struct stack_t *stack){
    if(stack==NULL||stack->capacity<1||stack->data==NULL||stack->head<0||stack->head>stack->capacity)
        return;
    for (int i=stack->head-1;i>-1;i--)
        printf("%d ",*(stack->data+i));
}

void stack_free(struct stack_t *stack){
    if(stack==NULL||stack->capacity<1||stack->data==NULL||stack->head<0||stack->head>stack->capacity)
        return;

    free(stack->data);
    free(stack);
}

