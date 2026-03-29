#include "circular_buffer.h"
#include <stdio.h>


int main() {
    int N;
    struct circular_buffer_t *cb;

    printf("Podaj rozmiar bufora:");
    if(scanf("%d",&N)!=1){
        printf("Incorrect input");
        return 1;
    }
    if(N<1){
        printf("Incorrect input data");
        return 2;
    }
    int wynik=circular_buffer_create_struct(&cb,N);
    if(wynik==1){
        printf("Incorrect input data");
        return 2;
    }
    if(wynik==2){
        printf("Failed to allocate memory");
        return 8;
    }

    int wybor,liczba,err_code;
    while (1){
        printf("Co chesz zrobic?");
        if(scanf("%d",&wybor)!=1){
            printf("Incorrect input");
            circular_buffer_destroy_struct(&cb);
            return 1;
        }
        if(wybor==0){
            circular_buffer_destroy_struct(&cb);
            return 0;
        }
        else if(wybor==1){
            printf("Podaj liczbe:");
            if(scanf("%d",&liczba)!=1){
                printf("Incorrect input");
                circular_buffer_destroy_struct(&cb);
                return 1;
            }
            circular_buffer_push_back(cb,liczba);
        }
        else if(wybor==2){
            if(circular_buffer_empty(cb)==1)
                printf("Buffer is empty\n");
            else
                printf("%d\n", circular_buffer_pop_back(cb,&err_code));
        }
        else if(wybor==3){
            if(circular_buffer_empty(cb)==1)
                printf("Buffer is empty\n");
            else
                printf("%d\n",circular_buffer_pop_front(cb,&err_code));
        }
        else if(wybor==4){
            if(circular_buffer_empty(cb)==1)
                printf("Buffer is empty\n");
            else
                circular_buffer_display(cb);
        }
        else if(wybor==5){
            printf("%d\n",circular_buffer_empty(cb));
        }
        else if(wybor==6){
            printf("%d\n",circular_buffer_full(cb));
        }
        else
            printf("Incorrect input data\n");
    }
}