#include "circular_buffer.h"
#include <stdlib.h>
#include <stdio.h>



int circular_buffer_create(struct circular_buffer_t *cb, int N) {
    if(cb==NULL||N<1)
        return 1;

    cb->ptr=(int *)malloc(N*sizeof(int));
    if(cb->ptr==NULL)
        return 2;

    cb->begin=0;
    cb->end=0;
    cb->capacity=N;
    cb->full=0;

    return 0;
}

int circular_buffer_create_struct(struct circular_buffer_t **cb, int N) {
    if(cb==NULL||N<1)
        return 1;

    *cb=(struct circular_buffer_t *)malloc(sizeof(struct circular_buffer_t));
    if (*cb==NULL)
        return 2;

    int wynik=circular_buffer_create(*cb, N);
    if(wynik!=0){
        free(*cb);
        return wynik;
    }
    return 0;
}

void circular_buffer_destroy(struct circular_buffer_t *cb) {
    if (cb==NULL)
        return;

    free(cb->ptr);
}

void circular_buffer_destroy_struct(struct circular_buffer_t **cb) {
    if(cb==NULL||*cb==NULL)
        return;

    circular_buffer_destroy(*cb);
    free(*cb);
}

int circular_buffer_push_back(struct circular_buffer_t *cb, int value) {
    if (cb==NULL||cb->ptr==NULL||cb->begin<0||cb->end<0||cb->capacity<1||cb->capacity<cb->begin||cb->capacity<cb->end)
        return 1;

    if(cb->full==1)
        cb->begin=(cb->begin+1)%cb->capacity;
    *(cb->ptr+cb->end)=value;
    cb->end=(cb->end+1)%cb->capacity;
    if(cb->end==cb->begin)
        cb->full=1;

    return 0;
}


int circular_buffer_pop_front(struct circular_buffer_t *cb, int *err_code) {
    if (err_code!=NULL) {
        if (cb==NULL||cb->begin<0||cb->end<0||cb->capacity<1||cb->capacity<cb->begin||cb->capacity<cb->end) {
            *err_code=1;
            return *err_code;
        }

        if ((circular_buffer_empty(cb)==1)) {
            *err_code=2;
            return *err_code;
        }

        int value=*(cb->ptr+cb->begin);
        cb->begin=(cb->begin+1)%cb->capacity;
        cb->full=0;

        *err_code=0;
        return value;
    }
    else{
        if (cb==NULL||cb->begin<0||cb->end<0||cb->capacity<1||cb->capacity<cb->begin||cb->capacity<cb->end)
            return 0;

        if ((circular_buffer_empty(cb)==1))
            return 2;

        int value=*(cb->ptr+cb->begin);
        cb->begin=(cb->begin+1)%cb->capacity;
        cb->full=0;

        return value;
    }
}

int circular_buffer_pop_back(struct circular_buffer_t *cb, int *err_code) {
    if (err_code!=NULL) {
        if (cb==NULL||cb->begin<0||cb->end<0||cb->capacity<1||cb->capacity<cb->begin||cb->capacity<cb->end) {
            *err_code=1;
            return *err_code;
        }

        if ((circular_buffer_empty(cb)==1)) {
            *err_code=2;
            return 2;
        }

        cb->end=(cb->end+cb->capacity-1)%cb->capacity;
        int value=*(cb->ptr+cb->end);
        cb->full=0;

        *err_code=0;
        return value;
    }
    else{
        if (cb==NULL||cb->begin<0||cb->end<0||cb->capacity<1||cb->capacity<cb->begin||cb->capacity<cb->end)
            return 0;

        if ((circular_buffer_empty(cb)==1))
            return 2;

        cb->end=(cb->end+cb->capacity-1)%cb->capacity;
        int value=*(cb->ptr+cb->end);
        cb->full=0;

        return value;
    }
}

int circular_buffer_empty(const struct circular_buffer_t *cb) {
    if (cb==NULL||cb->begin<0||cb->end<0||cb->capacity<1||cb->capacity<cb->begin||cb->capacity<cb->end)
        return -1;

    if(!cb->full&&(cb->begin==cb->end))
        return 1;
    else
        return 0;
}

int circular_buffer_full(const struct circular_buffer_t *cb) {
    if (cb==NULL||cb->begin<0||cb->end<0||cb->capacity<1||cb->capacity<cb->begin||cb->capacity<cb->end)
        return -1;

    return cb->full;
}

void circular_buffer_display(const struct circular_buffer_t *cb) {
    if (cb==NULL||cb->begin<0||cb->end<0||cb->capacity<1||cb->capacity<cb->begin||cb->capacity<cb->end)
        return;

    if (circular_buffer_empty(cb)==1)
        return;

    int poczatek=cb->begin;
    do {
        printf("%d ", *(cb->ptr+poczatek));
        poczatek=(poczatek+1)%cb->capacity;
    } while (poczatek!=cb->end);
    printf("\n");
}
