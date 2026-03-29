#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"
#include "tested_declarations.h"
#include "rdebug.h"

int main() {
    struct linked_list_t *lista=ll_create();

    if (lista==NULL) {
        printf("Failed to allocate memory");
        return 8;
    }

    int wybor,wynik;
    while (1){
        printf("Co chcesz zrobic?");
        if(scanf("%d",&wybor)!=1){
            printf("Incorrect input");
            ll_clear(lista);
            free(lista);
            return 1;
        }
        while (getchar()!='\n');

        if(wybor==0){
            ll_clear(lista);
            free(lista);
            return 0;
        }
        else if(wybor==1){
            int liczba;
            printf("Podaj liczbe");
            if(scanf("%d",&liczba)!=1){
                printf("Incorrect input");
                ll_clear(lista);
                free(lista);
                return 1;
            }
            while (getchar()!='\n');

            wynik=ll_push_back(lista,liczba);
            if(wynik==2){
                printf("Failed to allocate memory");
                ll_clear(lista);
                free(lista);
                return 8;
            }
        }
        else if(wybor==2){
            if(ll_is_empty(lista)==1)
                printf("List is empty\n");
            else{
                int liczba=ll_pop_back(lista,&wynik);
                printf("%d\n",liczba);
            }
        }
        else if(wybor==3){
            int liczba;
            printf("Podaj liczbe");
            if(scanf("%d",&liczba)!=1){
                printf("Incorrect input");
                ll_clear(lista);
                free(lista);
                return 1;
            }
            while (getchar()!='\n');

            wynik=ll_push_front(lista,liczba);
            if(wynik==2){
                printf("Failed to allocate memory");
                ll_clear(lista);
                free(lista);
                return 8;
            }
        }
        else if(wybor==4){
            if(ll_is_empty(lista)==1)
                printf("List is empty\n");
            else{
                int liczba=ll_pop_front(lista,&wynik);
                printf("%d\n",liczba);
            }
        }
        else if(wybor==5){
            int liczba;
            printf("Podaj liczbe");
            if(scanf("%d",&liczba)!=1){
                printf("Incorrect input");
                ll_clear(lista);
                free(lista);
                return 1;
            }
            while (getchar()!='\n');

            int index;
            printf("Podaj index");
            if(scanf("%d",&index)!=1){
                printf("Incorrect input");
                ll_clear(lista);
                free(lista);
                return 1;
            }
            while (getchar()!='\n');

            wynik=ll_insert(lista,index,liczba);
            if(wynik==1)
                printf("Index out of range\n");
            else if(wynik==2){
                printf("Failed to allocate memory");
                ll_clear(lista);
                free(lista);
                return 8;
            }
        }
        else if(wybor==6){
            if(ll_is_empty(lista)==1)
                printf("List is empty\n");
            else{
                int index;
                printf("Podaj index");
                if(scanf("%d",&index)!=1){
                    printf("Incorrect input");
                    ll_clear(lista);
                    free(lista);
                    return 1;
                }
                while (getchar()!='\n');

                int liczba=ll_remove(lista,index,&wynik);
                if(wynik==1)
                    printf("Index out of range\n");
                else
                    printf("%d\n",liczba);
            }
        }
        else if(wybor==7){
            if(ll_is_empty(lista)==1)
                printf("List is empty\n");
            else{
                int liczba=ll_back(lista,&wynik);
                printf("%d\n",liczba);
            }
        }
        else if(wybor==8){
            if(ll_is_empty(lista)==1)
                printf("List is empty\n");
            else{
                int liczba=ll_front(lista,&wynik);
                printf("%d\n",liczba);
            }
        }
        else if(wybor==9)
            printf("%d\n", ll_is_empty(lista));
        else if(wybor==10)
            printf("%d\n", ll_size(lista));
        else if(wybor==11)
            ll_clear(lista);
        else if(wybor==12){
            if(ll_is_empty(lista)==1)
                printf("List is empty\n");
            else{
                int index;
                printf("Podaj index");
                if(scanf("%d",&index)!=1){
                    printf("Incorrect input");
                    ll_clear(lista);
                    free(lista);
                    return 1;
                }
                while (getchar()!='\n');

                int liczba=ll_at(lista,index,&wynik);
                if(wynik==1)
                    printf("Index out of range\n");
                else
                    printf("%d\n",liczba);
            }
        }
        else if(wybor==13){
            if (ll_is_empty(lista)==1)
                printf("List is empty\n");
            else {
                ll_display(lista);
                printf("\n");
            }
        }
        else
            printf("Incorrect input data\n");
    }
}

