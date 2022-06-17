#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const short LEN = 1000;
const char SPACE = ' ';


typedef struct {
    short num;
    short * gpp;
} gaps;


void ScanfStr (char * str, short n);
short StrSkipCh (char * str, char c, short * i);
gaps StrChGaps (char * str, char c);
inline short Min (short n1, short n2);
inline void SwapAdr (short ** a1, short ** a2);
void SortAdr (short ** adr, short n);
void Evenly (gaps g, short ext);
char * CorrectStrCond (char * str, char c, short wd, gaps g);
char * CorrectStr (char * str, char c, short wd);



void PrintGaps (gaps g) { //!!!!!!!!!!!!!!!!!!
    short i;

    for (i = 0; i < g.num; i++)
        printf("%hd) %hd\n", i, g.gpp[i]);
    putchar('\n'); 
}



short PrintfStr (char * str) { //!!!!!!!!!!!!!!!!
    short i;

    for (i = 0; str[i] != '\0'; i++)
        putchar(str[i]);

    return i == 0 ? i : --i;
}



void ScanfStr (char * str, short n) {
    short i;
    char c; 
    
    for (i = 0, n--; (i < n) && ( (c = getchar()) != '\n'); i++) 
        str[i] = c;
    
    str[i] = '\0';
    while (c != '\n') 
        c = getchar();  
}


short StrSkipCh (char * str, char c, short * i) {
    short num;

    for (num = 0; (str[*i] == c) && (str[*i] != '\0'); num++, (*i)++) 
        ;
    return num;
}


gaps StrChGaps (char * str, char c) {
    gaps g;
    short num, i;

    for (i = 0, num = 0; str[i] != '\0'; i++) 
        if (str[i] == c) {
            StrSkipCh( str, c, &i);
            num++;
        }

    g.num = num;
    if (num != 0) {
        g.gpp = new short[num];
        for (i = 0, num = 0; str[i] != '\0'; i++)
            if (str[i] == c) {
                g.gpp[num] = StrSkipCh( str, c, &i);
                num++;
            }
    } else 
        g.gpp = nullptr;

    return g;
}


inline short Min (short n1, short n2) {
    return n1 < n2 ? n1 : n2; 
}


inline void SwapAdr (short ** a1, short ** a2) {
    short * ext;

    ext = *a1;
    *a1 = *a2;
    *a2 = ext;
}


void SortAdr (short ** adr, short n){
    short i;

    if (n > 1) {
        for (i = 0; i < n-1; i++)
            if (*adr[i] > *adr[i+1]) 
                SwapAdr(adr + i, adr + i + 1);
        SortAdr ( adr, n-1);
    } 
    
}



void Evenly (gaps g, short ext) {
    short i, j, dif;

    if (g.num != 0) {

        short arr[g.num];
        for (i = 0; i < g.num; i++)
            arr[i] = g.gpp[i];

        short * adr[g.num];
        for (i = 0; i < g.num; i++)
            adr[i] = g.gpp + i;
        SortAdr( adr, g.num);

        for (j = 1; j < g.num; j++) {
            dif = *adr[j] - *adr[0];
            for (i = 0; i < j; i++) {
                *adr[i] += Min(ext, dif);
                ext -= Min(dif, ext);
            }
        }

        for (dif = ext/g.num, i = 0; i < g.num; i++)
            g.gpp[i] += dif;
        
        for (ext %= g.num, i = 0; ext > 0; ext--, i++)                         
            g.gpp[i]++;

        for (i = 0; i < g.num; i++)
            g.gpp[i] -= arr[i];
    }
} 


char * CorrectStrCond (char * str, char c, short wd, gaps g) {
    char * cor;
    short i, j, k, gap;

    cor = new char[wd+1];
    cor[wd+1] = '\0';

    for (i = 0, j = 0, gap = 0; str[i] != '\0'; ) {
        if (str[i] != c) {
            cor[j] = str[i]; 
            i++;
            j++;
        }
        else {
            for ( ; (str[i] == c) && (str[i] != '\0'); i++, j++)
                cor[j] = c;
            for (k = 0; k < g.gpp[gap]; k++, j++)
                cor[j] = c;
            gap++;
        }  
    }

    return cor;
}  


char * CorrectStr (char * str, char c, short wd) {
    char * cor;
    short len;
    gaps g;

    len = strlen( str);
    g = StrChGaps( str, c);
    if ( (len >= wd) || (g.num == 0) ) {
        cor = new char[len + 1];
        strcpy( cor, str);
    } else {
        Evenly( g, wd - len);
        cor = CorrectStrCond( str, c, wd, g);
    }

    delete[] g.gpp;
    return cor;
}


short wd, am, i;


int main() {

    printf("\nEnter number of input lines: ");
    scanf("%hd", &am);
    if (am != 0) {
        printf("\nEnter prefered width: ");
        scanf("%hd", &wd);
    }
    else
        printf("\nNo line are to be entered.\n\n");
    getchar();

    char str[am][LEN], * cor[am];

    if (am != 0)
        printf("\nEnter the lines: \n");
    for (i = 0; i < am; i++) {
        printf("%d)", i+1);
        ScanfStr(str[i], LEN);
        cor[i] = CorrectStr(str[i], SPACE, wd);                               
    }    

    if (am != 0)
        printf("\nThe line:\n\n");
    for (i = 0; i < am; i++) 
        printf("%d)\nOriginal:\n%s\nAligned:\n%s\n\n", i+1, str[i], cor[i]);                                                                                    
    

    for (i = 0; i < am; i++) 
        delete[] cor[i];
    return 0;
}