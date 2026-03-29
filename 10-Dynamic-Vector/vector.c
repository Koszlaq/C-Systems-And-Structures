#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

int vector_create_struct(struct vector_t **a, int N){
    if(a==NULL||N<1)
        return 1;

    *a=(struct vector_t *)malloc(sizeof(struct vector_t));
    if (*a==NULL)
        return 2;

    int wynik=vector_create(*a,N);
    if(wynik!=0){
        free(*a);
        return wynik;
    }

    return 0;
}

void vector_destroy_struct(struct vector_t **a){
    if(a==NULL)
        return;

    vector_destroy(*a);
    free(*a);
}

int vector_create(struct vector_t *a, int N){
    if(a==NULL||N<1)
        return 1;

    a->ptr=(int *)malloc(N*sizeof(int));
    if (a->ptr==NULL)
        return 2;
    a->size=0;
    a->capacity=N;

    return 0;
}

void vector_destroy(struct vector_t *a){
    if(a==NULL)
        return;

    free(a->ptr);
}

void vector_display(const struct vector_t *a){
    if(a==NULL||a->ptr==NULL||a->capacity<1||a->size<1||a->size>a->capacity)
        return;

    for (int i=0;i<a->size;i++)
        printf("%d ", *(a->ptr+i));
    printf("\n");
}

int vector_push_back(struct vector_t *a, int value){
    if(a==NULL||a->capacity<1||a->size<0||a->size>a->capacity)
        return 1;

    if (a->size+1>a->capacity){
        int nowy_rozmiar=2*a->capacity;
        int *nowy=realloc(a->ptr,nowy_rozmiar*sizeof(int));
        if (nowy==NULL)
            return 2;
        a->ptr=nowy;
        a->capacity=nowy_rozmiar;
    }
    *(a->ptr+a->size)=value;
    a->size++;

    return 0;
}

int vector_erase(struct vector_t *a, int value){
    if(a==NULL||a->capacity<1||a->size<0||a->size>a->capacity)
        return -1;

    int ile=0;
    for (int i=0;i<a->size;i++) {
        if(*(a->ptr+i)==value)
            ile++;
        else
            *(a->ptr+(i-ile))=*(a->ptr+i);
    }

    a->size=a->size-ile;
    if(a->size<a->capacity/4){
        int nowy_rozmiar;
        if((a->capacity/2)>1)
            nowy_rozmiar=a->capacity/2;
        else
            nowy_rozmiar=1;
        int *nowy=realloc(a->ptr,nowy_rozmiar*sizeof(int));
        if (nowy==NULL)
            return -1;
        a->ptr=nowy;
        a->capacity=nowy_rozmiar;
    }

    return ile;
}
