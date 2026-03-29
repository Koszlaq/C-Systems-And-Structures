#include <stdio.h>
#include "array.h"

int main() {
    int N;
    float liczba;
    struct array_t *a;
    struct array_t *b;

    printf("Podaj rozmiar tablicy:");
    if(scanf("%d",&N)!=1){
        printf("Incorrect input");
        return 1;
    }
    if(N<1){
        printf("Incorrect input data");
        return 2;
    }

    int wynik=array_create_struct_float(&a,N);
    if(wynik==1){
        printf("Incorrect input data");
        return 2;
    }
    if(wynik==2){
        printf("Failed to allocate memory");
        return 8;
    }

    printf("Podaj liczby:");
    for (int i=0;i<N;i++){
        if(scanf("%f",&liczba)!=1){
            printf("Incorrect input");
            array_destroy_struct_float(&a);
            return 1;
        }
        if (array_push_back_float(a, liczba)==2){
            printf("Buffer is full\n");
            break;
        }
    }

    printf("Podaj rozmiar tablicy:");
    if(scanf("%d",&N)!=1){
        printf("Incorrect input");
        array_destroy_struct_float(&a);
        return 1;
    }
    if(N<1){
        printf("Incorrect input data");
        array_destroy_struct_float(&a);
        return 2;
    }

    int wynik2=array_create_struct_float(&b,N);
    if(wynik2==1){
        printf("Incorrect input data");
        array_destroy_struct_float(&a);
        return 2;
    }
    if(wynik2==2){
        printf("Failed to allocate memory");
        array_destroy_struct_float(&a);
        return 8;
    }

    printf("Podaj liczby:");
    for (int i=0;i<N;i++) {
        if(scanf("%f",&liczba)!=1){
            printf("Incorrect input");
            array_destroy_struct_float(&a);
            array_destroy_struct_float(&b);
            return 1;
        }
        if (array_push_back_float(b, liczba)==2){
            printf("Buffer is full\n");
            break;
        }
    }

    struct array_t *dodany_wektor;
    int dodawanie=add_vectors(a,b,&dodany_wektor);
    if(dodawanie==1)
        printf("Operation not permitted\n");
    else if(dodawanie==2){
        printf("Failed to allocate memory");
        array_destroy_struct_float(&a);
        array_destroy_struct_float(&b);
        return 8;
    }
    else{
        array_display_float(dodany_wektor);
        printf("\n");
        array_destroy_struct_float(&dodany_wektor);
    }

    struct array_t *odejmowany_wektor;
    int odejmowanie=subtract_vectors(a,b,&odejmowany_wektor);
    if(odejmowanie==1)
        printf("Operation not permitted\n");
    else if(odejmowanie==2){
        printf("Failed to allocate memory");
        array_destroy_struct_float(&a);
        array_destroy_struct_float(&b);
        return 8;
    }
    else{
        array_display_float(odejmowany_wektor);
        printf("\n");
        array_destroy_struct_float(&odejmowany_wektor);
    }

    float iloczyn_skalarny=dot_product(a,b);
    if(iloczyn_skalarny<0)
        printf("Operation not permitted\n");
    else
        printf("%.2f\n",iloczyn_skalarny);

    float kat_pomiedzy_wektorami=angle_between_vectors(a,b);
    if(kat_pomiedzy_wektorami<0)
        printf("Operation not permitted\n");
    else
        printf("%.2f\n",kat_pomiedzy_wektorami);

    float dlugosc_wektor1=length(a);
    if(dlugosc_wektor1<0)
        printf("Operation not permitted\n");
    else
        printf("%.2f\n",dlugosc_wektor1);

    float dlugosc_wektor2=length(b);
    if(dlugosc_wektor2<0)
        printf("Operation not permitted\n");
    else
        printf("%.2f\n",dlugosc_wektor2);

    struct array_t *znormalizowany_wektor_a;
    int normalizacja_wektora_a=normalize_vector(a,&znormalizowany_wektor_a);
    if(normalizacja_wektora_a==1||normalizacja_wektora_a==3)
        printf("Operation not permitted\n");
    else if(normalizacja_wektora_a==2){
        printf("Failed to allocate memory");
        array_destroy_struct_float(&a);
        array_destroy_struct_float(&b);
        return 8;
    }
    else{
        array_display_float(znormalizowany_wektor_a);
        printf("\n");
        array_destroy_struct_float(&znormalizowany_wektor_a);
    }

    struct array_t *znormalizowany_wektor_b;
    int normalizacja_wektora_b=normalize_vector(b,&znormalizowany_wektor_b);
    if(normalizacja_wektora_b==1||normalizacja_wektora_b==3)
        printf("Operation not permitted\n");
    else if(normalizacja_wektora_b==2){
        printf("Failed to allocate memory");
        array_destroy_struct_float(&a);
        array_destroy_struct_float(&b);
        return 8;
    }
    else{
        array_display_float(znormalizowany_wektor_b);
        printf("\n");
        array_destroy_struct_float(&znormalizowany_wektor_b);
    }


    array_destroy_struct_float(&a);
    array_destroy_struct_float(&b);
    return 0;
}
