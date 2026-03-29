#include <stdio.h>
#include <stdlib.h>
#include "doubly_linked_list.h"
#include "tested_declarations.h"
#include "rdebug.h"

int main() {
    struct doubly_linked_list_t *lista_dwukierunkowa=dll_create();

    if (lista_dwukierunkowa==NULL) {
        printf("Failed to allocate memory");
        return 8;
    }

    int wybor,wynik;
    while (1){
        printf("Co chcesz zrobic?");
        if(scanf("%d",&wybor)!=1){
            printf("Incorrect input");
            dll_clear(lista_dwukierunkowa);
            free(lista_dwukierunkowa);
            return 1;
        }
        while (getchar()!='\n');

        if(wybor==0){
            dll_clear(lista_dwukierunkowa);
            free(lista_dwukierunkowa);
            return 0;
        }
        else if(wybor==1){
            int liczba;
            printf("Podaj liczbe");
            if(scanf("%d",&liczba)!=1){
                printf("Incorrect input");
                dll_clear(lista_dwukierunkowa);
                free(lista_dwukierunkowa);
                return 1;
            }
            while (getchar()!='\n');

            wynik=dll_push_back(lista_dwukierunkowa,liczba);
            if(wynik==2){
                printf("Failed to allocate memory");
                dll_clear(lista_dwukierunkowa);
                free(lista_dwukierunkowa);
                return 8;
            }
        }
        else if(wybor==2){
            if(dll_is_empty(lista_dwukierunkowa)==1)
                printf("List is empty\n");
            else{
                int liczba=dll_pop_back(lista_dwukierunkowa,&wynik);
                printf("%d\n",liczba);
            }
        }
        else if(wybor==3){
            int liczba;
            printf("Podaj liczbe");
            if(scanf("%d",&liczba)!=1){
                printf("Incorrect input");
                dll_clear(lista_dwukierunkowa);
                free(lista_dwukierunkowa);
                return 1;
            }
            while (getchar()!='\n');

            wynik=dll_push_front(lista_dwukierunkowa,liczba);
            if(wynik==2){
                printf("Failed to allocate memory");
                dll_clear(lista_dwukierunkowa);
                free(lista_dwukierunkowa);
                return 8;
            }
        }
        else if(wybor==4){
            if(dll_is_empty(lista_dwukierunkowa)==1)
                printf("List is empty\n");
            else{
                int liczba=dll_pop_front(lista_dwukierunkowa,&wynik);
                printf("%d\n",liczba);
            }
        }
        else if(wybor==5){
            int liczba;
            printf("Podaj liczbe");
            if(scanf("%d",&liczba)!=1){
                printf("Incorrect input");
                dll_clear(lista_dwukierunkowa);
                free(lista_dwukierunkowa);
                return 1;
            }
            while (getchar()!='\n');

            int index;
            printf("Podaj index");
            if(scanf("%d",&index)!=1){
                printf("Incorrect input");
                dll_clear(lista_dwukierunkowa);
                free(lista_dwukierunkowa);
                return 1;
            }
            while (getchar()!='\n');

            wynik=dll_insert(lista_dwukierunkowa,index,liczba);
            if(wynik==1)
                printf("Index out of range\n");
            else if(wynik==2){
                printf("Failed to allocate memory");
                dll_clear(lista_dwukierunkowa);
                free(lista_dwukierunkowa);
                return 8;
            }
        }
        else if(wybor==6){
            if(dll_is_empty(lista_dwukierunkowa)==1)
                printf("List is empty\n");
            else{
                int index;
                printf("Podaj index");
                if(scanf("%d",&index)!=1){
                    printf("Incorrect input");
                    dll_clear(lista_dwukierunkowa);
                    free(lista_dwukierunkowa);
                    return 1;
                }
                while (getchar()!='\n');

                int liczba=dll_remove(lista_dwukierunkowa,index,&wynik);
                if(wynik==1)
                    printf("Index out of range\n");
                else
                    printf("%d\n",liczba);
            }
        }
        else if(wybor==7){
            if(dll_is_empty(lista_dwukierunkowa)==1)
                printf("List is empty\n");
            else{
                int liczba=dll_back(lista_dwukierunkowa,&wynik);
                printf("%d\n",liczba);
            }
        }
        else if(wybor==8){
            if(dll_is_empty(lista_dwukierunkowa)==1)
                printf("List is empty\n");
            else{
                int liczba=dll_front(lista_dwukierunkowa,&wynik);
                printf("%d\n",liczba);
            }
        }
        else if(wybor==9)
            printf("%d\n", dll_is_empty(lista_dwukierunkowa));
        else if(wybor==10)
            printf("%d\n", dll_size(lista_dwukierunkowa));
        else if(wybor==11)
            dll_clear(lista_dwukierunkowa);
        else if(wybor==12){
            if(dll_is_empty(lista_dwukierunkowa)==1)
                printf("List is empty\n");
            else{
                int index;
                printf("Podaj index");
                if(scanf("%d",&index)!=1){
                    printf("Incorrect input");
                    dll_clear(lista_dwukierunkowa);
                    free(lista_dwukierunkowa);
                    return 1;
                }
                while (getchar()!='\n');

                int liczba=dll_at(lista_dwukierunkowa,index,&wynik);
                if(wynik==1)
                    printf("Index out of range\n");
                else
                    printf("%d\n",liczba);
            }
        }
        else if(wybor==13){
            if (dll_is_empty(lista_dwukierunkowa)==1)
                printf("List is empty\n");
            else {
                dll_display(lista_dwukierunkowa);
                printf("\n");
            }
        }
        else if(wybor==14){
            if (dll_is_empty(lista_dwukierunkowa)==1)
                printf("List is empty\n");
            else {
                dll_display_reverse(lista_dwukierunkowa);
                printf("\n");
            }
        }
        else
            printf("Incorrect input data\n");
    }
}

