#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

struct point_t{
    int x;
    int y;
};

int dl_liczba(int n){
    int ile=0;

    while (n!=0){
        n=n/10;
        ile++;
    }

    return ile;
}

int cyfra(int liczba){
    int ile=0;

    if(liczba==0){
        putchar('0');
        ile++;
    }
    else if(liczba<0){
        putchar('-');
        ile++;
        liczba=liczba*(-1);
        int i=1,dl=dl_liczba(liczba);
        while (i<=dl){
            int cyfra=(int)(liczba/pow(10,dl_liczba(liczba)-i));
            cyfra=cyfra%10;
            putchar(cyfra+'0');
            ile++;
            i++;
        }
    }
    else{
        int i=1,dl=dl_liczba(liczba);
        while (i<=dl){
            int cyfra=(int)(liczba/pow(10,dl_liczba(liczba)-i));
            cyfra=cyfra%10;
            putchar(cyfra+'0');
            ile++;
            i++;
        }
    }

    return ile;
}

int my_printf(char *format, ...) {
    va_list argumenty;
    va_start(argumenty, format);
    int ile=0;

    while (*format!='\0'){
        if(*format=='%'){
            format++;
            switch (*format) {
                case 's':{
                    char *napis=va_arg(argumenty, char *);
                    while (*napis) {
                        putchar(*napis++);
                        ile++;
                    }
                    break;
                }
                case 'd':{
                    int liczba=va_arg(argumenty, int);
                    ile=ile+cyfra(liczba);
                    break;
                }
                case 'f':{
                    double liczba=va_arg(argumenty,double);
                    int cz_przed_przecinkiem=(int)liczba;
                    ile=ile+cyfra(cz_przed_przecinkiem);
                    putchar('.');
                    ile++;
                    double cz_po_przecinku=liczba-cz_przed_przecinkiem;
                    if(cz_po_przecinku<0)
                        cz_po_przecinku=cz_po_przecinku*(-1);
                    for (int i=0;i<5;i++) {
                        cz_po_przecinku=cz_po_przecinku*10;
                        int cyferka=(int)cz_po_przecinku;
                        putchar(cyferka+'0');
                        ile++;
                        cz_po_przecinku=cz_po_przecinku-cyferka;
                    }
                    break;
                }
                case 'p':{
                    struct point_t punkt=va_arg(argumenty, struct point_t);
                    putchar('(');
                    ile++;
                    ile=ile+cyfra(punkt.x);
                    putchar(' ');
                    ile++;
                    ile=ile+cyfra(punkt.y);
                    putchar(')');
                    ile++;
                    break;
                }
                default:
                    putchar('%');
                    putchar(*format);
                    ile=ile+2;
                    break;
            }
        }
        else{
            putchar(*format);
            ile++;
        }
        format++;
    }

    va_end(argumenty);
    return ile;
}

int my_scanf(char *format, ...) {
    va_list argumenty;
    va_start(argumenty, format);
    int ile=0;

    while (*format!='\0'){
        if(*format=='%'){
            format++;
            switch (*format) {
                case 'd':{
                    int *liczba=va_arg(argumenty, int *);
                    *liczba=0;
                    int flaga=1,znak;
                    if(znak==',')
                        break;

                    while ((znak=getchar())!=EOF&&(znak==' '||znak=='\n'||znak== '\t'));
                    if (znak=='-') {
                        flaga=-1;
                        znak=getchar();
                    }
                    if(isalpha(znak)||znak==',')
                        break;
                    while (znak!=EOF&&znak>='0'&&znak<= '9') {
                        *liczba=*liczba*10+(znak-'0');
                        znak=getchar();
                        if(isalpha(znak)||znak==',')
                            break;
                    }

                    *liczba*=flaga;
                    ile++;
                    break;
                }
                case 'f':{
                    double *liczba=va_arg(argumenty,double *);
                    *liczba=0;
                    double flaga=1,skala=1;
                    int znak,przecinek=0;

                    while ((znak=getchar())!=EOF&&(znak==' '||znak=='\n'||znak== '\t'));
                    if (znak=='-') {
                        flaga=-1;
                        znak=getchar();
                    }
                    if(isalpha(znak))
                        break;

                    while (znak!=EOF&&(znak>='0'&&znak<='9'||znak=='.')) {
                        if (znak=='.')
                            przecinek=1;
                        else {
                            *liczba=*liczba*10+(znak-'0');
                            if (przecinek==1)
                                skala=skala*10;
                        }
                        znak=getchar();
                        if(isalpha(znak))
                            break;
                    }

                    *liczba=*liczba*flaga/skala;
                    ile++;
                    break;
                }
                case 'p':{
                    struct point_t *punkt=va_arg(argumenty, struct point_t*);
                    while (getchar() != '(');
                    if(my_scanf("%d %d", &(punkt->x), &(punkt->y))==2)
                        ile=ile+2;
                    break;
                }
                default:
                    break;
            }
        }
        format++;
    }

    va_end(argumenty);
    return ile;
}


int main() {
    struct point_t a;
    my_printf("Wpisz punkt w postaci (x y): ");
    int res = my_scanf("%p", &a);
    my_printf("Wynik: [%d;%d;%d]", a.x, a.y, res);
    return 0;
}
