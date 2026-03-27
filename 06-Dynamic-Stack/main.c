#include <stdio.h>
#include "stack.h"
#include "tested_declarations.h"
#include "rdebug.h"

int main() {
    struct stack_t *stos;
    int wynik=stack_init(&stos, 10);

    if (wynik==2) {
        printf("Failed to allocate memory");
        return 8;
    }

    int wybor;
    while (1){
        printf("Co chcesz zrobic?");
        if(scanf("%d",&wybor)!=1){
            printf("Incorrect input");
            stack_free(stos);
            return 1;
        }
        while (getchar()!='\n');

        if(wybor==0){
            stack_free(stos);
            return 0;
        }
        else if(wybor==1){
            int liczba;
            printf("Podaj liczbe");
            if(scanf("%d",&liczba)!=1){
                printf("Incorrect input");
                stack_free(stos);
                return 1;
            }
            while (getchar()!='\n');

            wynik=stack_push(stos,liczba);
            if(wynik==2){
                printf("Failed to allocate memory");
                stack_free(stos);
                return 8;
            }
        }
        else if(wybor==2){
            int liczba=stack_pop(stos,&wynik);
            if(wynik==2)
                printf("Stack is empty\n");
            else
                printf("%d\n",liczba);
        }
        else if(wybor==3){
            if(stos->head==0)
                printf("Stack is empty\n");
            else{
                stack_display(stos);
                printf("\n");
            }
        }
        else
            printf("Incorrect input data\n");
    }
}

