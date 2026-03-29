#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "array.h"

int array_create_float(struct array_t *a, int N){
    if(a==NULL||N<1)
        return 1;

    a->ptr=(float *)malloc(N*sizeof(float ));
    if(a->ptr==NULL)
        return 2;

    a->size=0;
    a->capacity=N;

    return 0;
}

int array_push_back_float(struct array_t *a, float  value){
    if (a==NULL||a->ptr==NULL||a->size<0||a->capacity<1||a->size>a->capacity)
        return 1;

    if (a->size==a->capacity)
        return 2;

    *(a->ptr+a->size)=value;
    a->size++;

    return 0;
}

void array_display_float(const struct array_t *a){
    if(a==NULL||a->ptr==NULL||a->size<1||a->size>a->capacity)
        return;

    for (int i=0;i<a->size;i++)
        printf("%.2f ",*(a->ptr+i));
}

void array_destroy_float(struct array_t *a){
    if(a==NULL||a->ptr==NULL)
        return;

    free(a->ptr);
    a->size=0;
    a->capacity=0;
}

void array_destroy_struct_float(struct array_t **a){
    if(a==NULL)
        return;

    array_destroy_float(*a);
    free(*a);
}

int array_create_struct_float(struct array_t **a, int N){
    if(a==NULL||N<1)
        return 1;

    *a=(struct array_t *)malloc(sizeof(struct array_t));
    if(*a==NULL)
        return 2;

    int wynik=array_create_float(*a,N);
    if(wynik!=0){
        free(*a);
        return wynik;
    }

    return 0;
}

int add_vectors(const struct array_t *a, const struct array_t *b, struct array_t **c){
    if(a==NULL||a->ptr==NULL||b==NULL||b->ptr==NULL||c==NULL||a->size!=b->size||a->size<0||b->size<0||a->capacity<1||b->capacity<1||a->size>a->capacity||b->size>b->capacity)
        return 1;

    int wynik=array_create_struct_float(c,a->size);
    if(wynik!=0)
        return 2;

    for (int i=0;i<a->size;i++)
        array_push_back_float(*c,*(a->ptr+i)+*(b->ptr+i));

    return 0;
}

int subtract_vectors(const struct array_t *a, const struct array_t *b, struct array_t **c){
    if(a==NULL||a->ptr==NULL||b==NULL||b->ptr==NULL||c==NULL||a->size!=b->size||a->size<0||b->size<0||a->capacity<1||b->capacity<1||a->size>a->capacity||b->size>b->capacity)
        return 1;

    int wynik=array_create_struct_float(c,a->size);
    if(wynik!=0)
        return 2;

    for (int i=0;i<a->size;i++)
        array_push_back_float(*c,*(a->ptr+i)-*(b->ptr+i));

    return 0;
}

float dot_product(const struct array_t *a, const struct array_t *b){
    if(a==NULL||a->ptr==NULL||b==NULL||b->ptr==NULL||a->size!=b->size||a->size<0||b->size<0||a->capacity<1||b->capacity<1||a->size>a->capacity||b->size>b->capacity)
        return -1;

    float iloczyn_skalarny=0;
    for (int i=0;i<a->size;i++)
        iloczyn_skalarny=iloczyn_skalarny+(*(a->ptr+i)**(b->ptr+i));

    return iloczyn_skalarny;
}

float angle_between_vectors(const struct array_t *a, const struct array_t *b){
    if(a==NULL||a->ptr==NULL||b==NULL||b->ptr==NULL||a->size!=b->size||a->size<0||b->size<0||a->capacity<1||b->capacity<1||a->size>a->capacity||b->size>b->capacity)
        return -1;

    float dlugosc=(length(a)*length(b));
    if(dlugosc==0)
        return -1;

    float kat;
    kat=(float)(acosf(dot_product(a,b)/dlugosc)*(180.0 / M_PI));

    return kat;
}

float length(const struct array_t *a){
    if(a==NULL||a->ptr==NULL||a->size<0||a->capacity<1||a->size>a->capacity)
        return -1;

    float dlugosc_wektora=0;
    for (int i=0;i<a->size;i++)
        dlugosc_wektora=(float)(dlugosc_wektora+pow(*(a->ptr+i),2));
    dlugosc_wektora=(float)sqrtf(dlugosc_wektora);

    return dlugosc_wektora;
}

int normalize_vector(const struct array_t *a, struct array_t **b){
    if(a==NULL||a->ptr==NULL||b==NULL||a->size<0||a->capacity<1)
        return 1;

    int wynik=array_create_struct_float(b,a->size);
    if(wynik!=0)
        return 2;

    float dlugosc=length(a);
    if(dlugosc==0){
        array_destroy_struct_float(b);
        return 3;
    }
    else if(dlugosc<0){
        array_destroy_struct_float(b);
        return 1;
    }

    for (int i=0;i<a->size;i++)
        array_push_back_float(*b,*(a->ptr+i)/dlugosc);

    return 0;
}

