#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"
#include "tested_declarations.h"
#include "rdebug.h"

struct linked_list_t* ll_create(){
    struct linked_list_t* lista_jednokierunkowa=malloc(sizeof(struct linked_list_t));
    if (lista_jednokierunkowa==NULL)
        return NULL;

    lista_jednokierunkowa->head=NULL;
    lista_jednokierunkowa->tail=NULL;
    return lista_jednokierunkowa;
}

int ll_push_back(struct linked_list_t* ll, int value){
    if(ll==NULL)
        return 1;

    struct node_t* nowe_dane=malloc(sizeof(struct node_t));
    if (nowe_dane==NULL)
        return 2;

    nowe_dane->data=value;
    nowe_dane->next=NULL;

    if (ll->tail!=NULL)
        ll->tail->next=nowe_dane;
    else
        ll->head=nowe_dane;

    ll->tail=nowe_dane;

    return 0;
}

int ll_push_front(struct linked_list_t* ll, int value){
    if(ll==NULL)
        return 1;

    struct node_t* nowe_dane=malloc(sizeof(struct node_t));
    if (nowe_dane==NULL)
        return 2;

    nowe_dane->data=value;
    nowe_dane->next=ll->head;
    ll->head=nowe_dane;

    if (ll->tail==NULL)
        ll->tail=nowe_dane;

    return 0;
}

int ll_pop_front(struct linked_list_t* ll, int *err_code){
    if(err_code!=NULL){
        if(ll==NULL||(ll->head==NULL&&ll->tail==NULL)==1){
            *err_code=1;
            return 1;
        }

        struct node_t* do_usuniecia=ll->head;
        int liczba=do_usuniecia->data;

        ll->head=ll->head->next;
        if (ll->head==NULL)
            ll->tail=NULL;
        free(do_usuniecia);

        *err_code=0;
        return liczba;
    }
    else{
        if(ll==NULL||(ll->head==NULL&&ll->tail==NULL)==1)
            return 1;

        struct node_t* do_usuniecia=ll->head;
        int liczba=do_usuniecia->data;

        ll->head=ll->head->next;
        if (ll->head==NULL)
            ll->tail=NULL;
        free(do_usuniecia);

        return liczba;
    }
}

int ll_pop_back(struct linked_list_t* ll, int *err_code){
    if(err_code!=NULL){
        if(ll==NULL||(ll->head==NULL&&ll->tail==NULL)==1){
            *err_code=1;
            return 1;
        }

        struct node_t* aktualne_dane=ll->head;
        if (aktualne_dane->next==NULL) {
            int liczba=aktualne_dane->data;
            free(aktualne_dane);

            ll->head=NULL;
            ll->tail=NULL;

            *err_code=0;
            return liczba;
        }

        while (aktualne_dane->next!=ll->tail)
            aktualne_dane=aktualne_dane->next;

        int liczba=ll->tail->data;
        free(ll->tail);
        ll->tail=aktualne_dane;
        aktualne_dane->next=NULL;

        *err_code=0;
        return liczba;
    }
    else{
        if(ll==NULL||(ll->head==NULL&&ll->tail==NULL)==1)
            return 1;

        struct node_t* aktualne_dane=ll->head;
        if (aktualne_dane->next==NULL) {
            int liczba=aktualne_dane->data;
            free(aktualne_dane);

            ll->head=NULL;
            ll->tail=NULL;

            return liczba;
        }

        while (aktualne_dane->next!=ll->tail)
            aktualne_dane=aktualne_dane->next;

        int liczba=ll->tail->data;
        free(ll->tail);
        ll->tail=aktualne_dane;
        aktualne_dane->next=NULL;

        return liczba;
    }
}

int ll_back(const struct linked_list_t* ll, int *err_code){
    if(err_code!=NULL){
        if (ll==NULL||(ll->head==NULL&&ll->tail==NULL)==1) {
            *err_code=1;
            return -1;
        }

        *err_code=0;
        return ll->tail->data;
    }
    else{
        if (ll==NULL||(ll->head==NULL&&ll->tail==NULL)==1)
            return -1;

        return ll->tail->data;
    }
}

int ll_front(const struct linked_list_t* ll, int *err_code){
    if(err_code!=NULL){
        if (ll==NULL||(ll->head==NULL&&ll->tail==NULL)==1) {
            *err_code=1;
            return -1;
        }

        *err_code=0;
        return ll->head->data;
    }
    else{
        if (ll==NULL||(ll->head==NULL&&ll->tail==NULL)==1)
            return -1;

        return ll->head->data;
    }
}

struct node_t* ll_begin(struct linked_list_t* ll){
    if(ll==NULL||ll->head==NULL)
        return NULL;

    return ll->head;
}

struct node_t* ll_end(struct linked_list_t* ll){
    if(ll==NULL||ll->tail==NULL)
        return NULL;

    return ll->tail;
}

int ll_size(const struct linked_list_t* ll){
    if (ll==NULL)
        return -1;

    int rozmiar=0;
    struct node_t* aktualne_dane=ll->head;

    while (aktualne_dane!=NULL) {
        rozmiar++;
        aktualne_dane=aktualne_dane->next;
    }

    return rozmiar;
}

int ll_is_empty(const struct linked_list_t* ll){
    if(ll==NULL)
        return -1;

    if(ll->head==NULL)
        return 1;
    else
        return 0;
}

int ll_at(const struct linked_list_t* ll, unsigned int index, int *err_code){
    if(err_code!=NULL){
        if (ll==NULL) {
            *err_code=1;
            return 1;
        }

        struct node_t* aktualne_dane=ll->head;
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
        if (ll==NULL)
            return 1;

        struct node_t* aktualne_dane=ll->head;
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

int ll_insert(struct linked_list_t* ll, unsigned int index, int value){
    if (ll==NULL)
        return 1;

    if (index==0)
        return ll_push_front(ll, value);

    struct node_t* aktualne_dane=ll->head;
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
    aktualne_dane->next=nowe_dane;

    if (nowe_dane->next==NULL)
        ll->tail=nowe_dane;

    return 0;
}

int ll_remove(struct linked_list_t* ll, unsigned int index, int *err_code){
    if(err_code!=NULL){
        if(ll==NULL){
            *err_code=1;
            return 1;
        }

        if (index==0)
            return ll_pop_front(ll, err_code);

        struct node_t* aktualne_dane=ll->head;
        for (unsigned int i=0;i<index-1;i++) {
            if (aktualne_dane==NULL||aktualne_dane->next==NULL) {
                *err_code=1;
                return 1;
            }
            aktualne_dane=aktualne_dane->next;
        }

        if (aktualne_dane->next==NULL) {
            *err_code=1;
            return 1;
        }

        struct node_t* do_usuniecia=aktualne_dane->next;
        int liczba=do_usuniecia->data;
        aktualne_dane->next=do_usuniecia->next;
        if (aktualne_dane->next==NULL)
            ll->tail=aktualne_dane;
        free(do_usuniecia);

        *err_code=0;
        return liczba;
    }
    else{
        if(ll==NULL)
            return 1;

        if (index==0)
            return ll_pop_front(ll, err_code);

        struct node_t* aktualne_dane=ll->head;
        for (unsigned int i=0;i<index-1;i++) {
            if (aktualne_dane==NULL||aktualne_dane->next==NULL)
                return 1;
            aktualne_dane=aktualne_dane->next;
        }

        if (aktualne_dane->next==NULL)
            return 1;

        struct node_t* do_usuniecia=aktualne_dane->next;
        int liczba=do_usuniecia->data;
        aktualne_dane->next=do_usuniecia->next;
        if (aktualne_dane->next==NULL)
            ll->tail=aktualne_dane;
        free(do_usuniecia);

        return liczba;
    }
}

void ll_clear(struct linked_list_t* ll){
    if (ll==NULL)
        return;

    struct node_t* aktualne_dane=ll->head;
    while (aktualne_dane!=NULL) {
        struct node_t* do_usuniecia=aktualne_dane;
        aktualne_dane=aktualne_dane->next;
        free(do_usuniecia);
    }

    ll->head=NULL;
    ll->tail=NULL;
}

void ll_display(const struct linked_list_t* ll){
    if(ll==NULL)
        return;

    struct node_t *aktualne_dane=ll->head;
    while (aktualne_dane!=NULL){
        printf("%d ",aktualne_dane->data);
        aktualne_dane=aktualne_dane->next;
    }
}
