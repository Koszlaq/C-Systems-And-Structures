#include <stdio.h>
#include "vector.h"

int main() {
    int N;
    struct vector_t *vector;

    printf("Podaj pojemnosc tablicy:");
    if(scanf("%d",&N)!=1){
        printf("Incorrect input");
        return 1;
    }
    if(N<1){
        printf("Incorrect input data");
        return 2;
    }

    int wynik=vector_create_struct(&vector,N);
    if(wynik==1){
        printf("Incorrect input data");
        return 2;
    }
    if(wynik==2){
        printf("Failed to allocate memory");
        return 8;
    }

    int wybor,liczba;
    while (1){
        printf("Co chesz zrobic?");
        if(scanf("%d",&wybor)!=1){
            printf("Incorrect input");
            vector_destroy_struct(&vector);
            return 1;
        }
        if(wybor==0){
            vector_destroy_struct(&vector);
            return 0;
        }
        else if(wybor==1){
            printf("Podaj kolejne liczby, ktore maja zostac dodane do tablicy:");
            while (1){
                if(scanf("%d",&liczba)!=1){
                    printf("Incorrect input");
                    vector_destroy_struct(&vector);
                    return 1;
                }
                if(liczba==0)
                    break;
                int wynik_dodania_do_tablicy=vector_push_back(vector,liczba);
                if(wynik_dodania_do_tablicy==1){
                    printf("Incorrect input data\n");
                    break;
                }
                if(wynik_dodania_do_tablicy==2){
                    printf("Failed to allocate memory\n");
                    break;
                }
            }
            if(vector->size==0)
                printf("Buffer is empty\n");
            else
                vector_display(vector);
        }
        else if(wybor==2){
            printf("Podaj kolejne liczby, ktore maja zostac usuniete z tablicy:");
            while (1){
                if(scanf("%d",&liczba)!=1){
                    printf("Incorrect input");
                    vector_destroy_struct(&vector);
                    return 1;
                }
                if(liczba==0)
                    break;
                int wynik_usuwania=vector_erase(vector,liczba);
                if(wynik_usuwania==-1){
                    printf("Incorrect input data\n");
                    break;
                }
            }
            if(vector->size==0)
                printf("Buffer is empty\n");
            else
                vector_display(vector);
        }
        else
            printf("Incorrect input data\n");
    }
}
