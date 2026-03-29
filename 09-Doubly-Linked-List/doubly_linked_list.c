#include <stdio.h>
#include <stdlib.h>
#include "doubly_linked_list.h"
#include "tested_declarations.h"
#include "rdebug.h"

struct doubly_linked_list_t* dll_create(){
    struct doubly_linked_list_t* lista_jednokierunkowa=malloc(sizeof(struct doubly_linked_list_t));
    if (lista_jednokierunkowa==NULL)
        return NULL;

    lista_jednokierunkowa->head=NULL;
    lista_jednokierunkowa->tail=NULL;
    return lista_jednokierunkowa;
}

int dll_push_back(struct doubly_linked_list_t* dll, int value){
    if(dll==NULL)
        return 1;

    struct node_t* nowe_dane=malloc(sizeof(struct node_t));
    if (nowe_dane==NULL)
        return 2;

    nowe_dane->data=value;
    nowe_dane->next=NULL;
    nowe_dane->prev=dll->tail;

    if (dll->tail!=NULL)
        dll->tail->next=nowe_dane;

    dll->tail=nowe_dane;

    if (dll->head==NULL)
        dll->head=nowe_dane;

    return 0;
}

int dll_push_front(struct doubly_linked_list_t* dll, int value){
    if(dll==NULL)
        return 1;

    struct node_t* nowe_dane=malloc(sizeof(struct node_t));
    if (nowe_dane==NULL)
        return 2;

    nowe_dane->data=value;
    nowe_dane->prev=NULL;
    nowe_dane->next=dll->head;

    if (dll->head!=NULL)
        dll->head->prev=nowe_dane;

    dll->head=nowe_dane;

    if (dll->tail==NULL)
        dll->tail=nowe_dane;

    return 0;
}

int dll_pop_front(struct doubly_linked_list_t* dll, int *err_code){
    if(err_code!=NULL){
     if(dll==NULL||(dll->head==NULL&&dll->tail==NULL)==1){
         *err_code=1;
         return 1;
     }

     struct node_t* do_usuniecia=dll->head;
     int liczba=do_usuniecia->data;
     dll->head=do_usuniecia->next;

     if(dll->head!=NULL)
         dll->head->prev=NULL;
     else
         dll->tail=NULL;
     free(do_usuniecia);

     *err_code=0;
     return liczba;
    }
    else{
        if(dll==NULL||(dll->head==NULL&&dll->tail==NULL)==1)
            return 1;

        struct node_t* do_usuniecia=dll->head;
        int liczba=do_usuniecia->data;
        dll->head=do_usuniecia->next;

        if(dll->head!=NULL)
            dll->head->prev=NULL;
        else
            dll->tail=NULL;
        free(do_usuniecia);

        return liczba;
    }
}

int dll_pop_back(struct doubly_linked_list_t* dll, int *err_code){
    if(err_code!=NULL){
        if(dll==NULL||(dll->head==NULL&&dll->tail==NULL)==1){
            *err_code=1;
            return 1;
        }

        struct node_t* do_usuniecia=dll->tail;
        int liczba=do_usuniecia->data;
        dll->tail=do_usuniecia->prev;

        if(dll->tail!=NULL)
            dll->tail->next=NULL;
        else
            dll->head=NULL;
        free(do_usuniecia);

        *err_code=0;
        return liczba;
    }
    else{
        if(dll==NULL||(dll->head==NULL&&dll->tail==NULL)==1)
            return 1;

        struct node_t* do_usuniecia=dll->tail;
        int liczba=do_usuniecia->data;
        dll->tail=do_usuniecia->prev;

        if(dll->tail!=NULL)
            dll->tail->next=NULL;
        else
            dll->head=NULL;
        free(do_usuniecia);

        return liczba;
    }
}

int dll_back(const struct doubly_linked_list_t* dll, int *err_code){
    if(err_code!=NULL){
        if(dll==NULL||(dll->head==NULL&&dll->tail==NULL)==1){
            *err_code=1;
            return 1;
        }

        *err_code=0;
        return dll->tail->data;
    }
    else{
        if(dll==NULL||(dll->head==NULL&&dll->tail==NULL)==1)
            return 1;

        return dll->tail->data;
    }
}

int dll_front(const struct doubly_linked_list_t* dll, int *err_code){
    if(err_code!=NULL){
        if(dll==NULL||(dll->head==NULL&&dll->tail==NULL)==1){
            *err_code=1;
            return 1;
        }

        *err_code=0;
        return dll->head->data;
    }
    else{
        if(dll==NULL||(dll->head==NULL&&dll->tail==NULL)==1)
            return 1;

        return dll->head->data;
    }
}

struct node_t* dll_begin(struct doubly_linked_list_t* dll){
    if(dll==NULL||(dll->head==NULL&&dll->tail==NULL)==1)
        return NULL;

    return dll->head;
}

struct node_t* dll_end(struct doubly_linked_list_t* dll){
    if(dll==NULL||(dll->head==NULL&&dll->tail==NULL)==1)
        return NULL;

    return dll->tail;
}

int dll_size(const struct doubly_linked_list_t* dll){
    if(dll==NULL)
        return -1;

    int rozmiar=0;
    struct node_t* aktualne_dane=dll->head;

    while (aktualne_dane!=NULL) {
        rozmiar++;
        aktualne_dane=aktualne_dane->next;
    }

    return rozmiar;
}
int dll_is_empty(const struct doubly_linked_list_t* dll){
    if(dll==NULL)
        return -1;

    if(dll->head==NULL)
        return 1;
    else
        return 0;
}

int dll_at(const struct doubly_linked_list_t* dll, unsigned int index, int *err_code){
    if(err_code!=NULL){
        if(dll==NULL||(dll->head==NULL&&dll->tail==NULL)==1){
            *err_code=1;
            return 1;
        }

        struct node_t* aktualne_dane=dll->head;
        for (unsigned int i=0;i<index;i++) {
            if (aktualne_dane==NULL){
                *err_code=1;
                return 1;
            }
            aktualne_dane=aktualne_dane->next;
        }

        if (aktualne_dane==NULL) {
            *err_code = 1;
            return 1;
        }

        *err_code=0;
        return aktualne_dane->data;

    }
    else{
        if(dll==NULL||(dll->head==NULL&&dll->tail==NULL)==1)
            return 1;

        struct node_t* aktualne_dane=dll->head;
        for (unsigned int i=0;i<index;i++) {
            if (aktualne_dane==NULL)
                return 1;
            aktualne_dane=aktualne_dane->next;
        }

        if (aktualne_dane==NULL)
            return 1;

        return aktualne_dane->data;
    }
}

int dll_insert(struct doubly_linked_list_t* dll, unsigned int index, int value){
    if(dll==NULL)
        return 1;

    if (index==0)
        return dll_push_front(dll, value);

    struct node_t* aktualne_dane=dll->head;
    for (unsigned int i=0;i<index-1;i++) {
        if (aktualne_dane==NULL)
            return 1;
        aktualne_dane=aktualne_dane->next;
    }

    if (aktualne_dane==NULL)
        return 1;

    struct node_t* nowe_dane=malloc(sizeof(struct node_t));
    if (nowe_dane==NULL)
        return 2;

    nowe_dane->data=value;
    nowe_dane->next=aktualne_dane->next;
    nowe_dane->prev=aktualne_dane;

    if(aktualne_dane->next!=NULL)
        aktualne_dane->next->prev=nowe_dane;

    aktualne_dane->next=nowe_dane;

    if (nowe_dane->next==NULL)
        dll->tail=nowe_dane;

    return 0;
}

int dll_remove(struct doubly_linked_list_t* dll, unsigned int index, int *err_code){
    if(err_code!=NULL){
        if(dll==NULL){
            *err_code=1;
            return 1;
        }

        struct node_t* aktualne_dane=dll->head;
        for (unsigned int i=0;aktualne_dane&&i<index;i++)
            aktualne_dane = aktualne_dane->next;

        if (aktualne_dane==NULL) {
            *err_code=1;
            return 1;
        }

        int liczba=aktualne_dane->data;
        if(aktualne_dane->prev!=NULL)
            aktualne_dane->prev->next=aktualne_dane->next;
        else
            dll->head=aktualne_dane->next;

        if (aktualne_dane->next!=NULL)
            aktualne_dane->next->prev=aktualne_dane->prev;
        else
            dll->tail=aktualne_dane->prev;
        free(aktualne_dane);


        *err_code=0;
        return liczba;
    }
    else{
        if(dll==NULL)
            return 1;

        struct node_t* aktualne_dane=dll->head;
        for (unsigned int i=0;aktualne_dane&&i<index;i++)
            aktualne_dane = aktualne_dane->next;

        if (aktualne_dane==NULL)
            return 1;
        
        int liczba=aktualne_dane->data;
        if(aktualne_dane->prev!=NULL)
            aktualne_dane->prev->next=aktualne_dane->next;
        else
            dll->head=aktualne_dane->next;

        if (aktualne_dane->next!=NULL)
            aktualne_dane->next->prev=aktualne_dane->prev;
        else
            dll->tail=aktualne_dane->prev;
        free(aktualne_dane);

        return liczba;
    }
}


void dll_clear(struct doubly_linked_list_t* dll){
    if(dll==NULL||(dll->head==NULL&&dll->tail==NULL)==1)
        return;

    struct node_t* aktualne_dane=dll->head;
    while (aktualne_dane!=NULL) {
        struct node_t* do_usuniecia=aktualne_dane;
        aktualne_dane=aktualne_dane->next;
        free(do_usuniecia);
    }

    dll->head=NULL;
    dll->tail=NULL;
}

void dll_display(const struct doubly_linked_list_t* dll){
    if(dll==NULL||(dll->head==NULL&&dll->tail==NULL)==1)
        return;

    struct node_t *aktualne_dane=dll->head;
    while (aktualne_dane!=NULL){
        printf("%d ",aktualne_dane->data);
        aktualne_dane=aktualne_dane->next;
    }
}

void dll_display_reverse(const struct doubly_linked_list_t* dll){
    if(dll==NULL||(dll->head==NULL&&dll->tail==NULL)==1)
        return;

    struct node_t *aktualne_dane=dll->tail;
    while (aktualne_dane!=NULL){
        printf("%d ",aktualne_dane->data);
        aktualne_dane=aktualne_dane->prev;
    }
}
