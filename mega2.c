#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
http://ganharnaloteria.com.br/2018/03/09/ganhar-na-mega-sena-aprenda-jogar-na-mega-sena-da-forma-correta/
https://confiraloterias.com.br/megasena/conferencia/
*/

#define N       6
#define SENA    60

typedef struct {
    int numeros[N];
} jogo;

/* ----------------------------------------------------------------- */

void imprime(int v[], int n)
{
    int i;

    for(i = 0; i < n; i++) {
        printf("%02d%s", v[i], i < n - 1 ? " - " : "");
    }
    printf("\n");
}

int esta_em(int e, int v[], int n)
{
    int i, achou = 0;

    for(i = 0; i < n; i++) {
        if(e == v[i]) {
            achou = 1;
            break;
        }
    }

    return achou;
}

int compara(const void *a, const void *b)
{
    int *na = (int *)a;
    int *nb = (int *)b;

    return (*na - *nb);
}

/* ----------------------------------------------------------------- */

void preenche_jogo(jogo *j)
{
    int i, n, c;
    int cols[N];

    /* primeiro, escolhe N colunas para serem sorteadas */
    if(N <= 10) {
        for(i = 0; i < N; i++) {
            do {
                n = rand() % 10 + 1; /* existem 10 colunas */
                cols[i] = n;
            } while(esta_em(n, cols, i));
        }
    }
	/* N > 10 e só temos 10 colunas */
	else {
		/* escolhemos todas */
        for(i = 0; i < 10; i++) {
            n = i + 1;
            cols[i] = n;
        }
		/* e sorteamos o que falta para N para serem repetidas */
        for(i = 10; i < N; i++) {
            n = rand() % 10 + 1; /* existem 10 colunas */
            cols[i] = n;
        }
    }
    qsort(cols, N, sizeof(int), compara);

    for(i = 0; i < N; i++) {
        do {
            c = rand() % 6 + 1; /* existem 6 números possíveis dentro de cada coluna */
            n = 10 * (c - 1) + cols[i]; /* calcula o c-ésimo número dessa coluna (cols[i]) */
            j->numeros[i] = n;
        } while(esta_em(n, j->numeros, i));
    }
    qsort(j->numeros, N, sizeof(int), compara);
}

/* ----------------------------------------------------------------- */

int termina_em_9_ou_0(jogo *j)
{
    /*
    static int proibidos[] = { 9, 10, 19, 20, 29, 30, 39, 40, 49, 50, 59, 60 };
    */
    static int proibidos[] = { 9, 10, 19, 20,     30, 39, 40, 49, 50, 59     }; /* 29 e 60 são dezenas de ouro */
    int i, n, termina = 0;

    for(i = 0; i < N; i++) {
        n = j->numeros[i];
        if(esta_em(n, proibidos, 10)) { /* temos 10 números proibidos */
            termina = 1;
            break;
        }
    }

    return termina;
}

int eh_dupla(jogo *j)
{
    static int proibidos[] = { 11, 22, 33, 44, 55 };
    int i, n, dupla = 0;

    for(i = 0; i < N; i++) {
        n = j->numeros[i];
        if(esta_em(n, proibidos, 5)) { /* temos 5 números proibidos */
            dupla = 1;
            break;
        }
    }

    return dupla;
}

int tem_seguidos(jogo *j)
{
    int i, n1, n2, tem = 0;

    for(i = 0; i < N - 1; i++) {
        n1 = j->numeros[i];
        n2 = j->numeros[i + 1];
        if(n2 == n1 + 1) {
            tem = 1;
            break;
        }
    }

    return tem;
}

int termina_com_mesmo_digito(int n1, int n2)
{
    char s1[10], s2[10];
    sprintf(s1, "%02d", n1);
    sprintf(s2, "%02d", n2);
    return s1[1] == s2[1];
}

int tem_mesma_coluna(jogo *j)
{
    int i, k, n1, n2, tem = 0;

    if(N > 10) {
        return tem;
    }

    for(i = 0; i < N - 1; i++) {
        n1 = j->numeros[i];
        for(k = i + 1; k < N; k++) {
            n2 = j->numeros[k];
            if(termina_com_mesmo_digito(n1, n2)) {
                tem = 1;
                goto fim; /* sai do laço mais externo */
            }
        }
    }
fim:

    return tem;
}

int tem_equilibrio_quadrantes(jogo *j)
{
    int q1[] = {   1,  2,  3,  4,  5,
                   11, 12, 13, 14, 15,
                   21, 22, 23, 24, 25
               };
    int q2[] = {   6,  7,  8,  9, 10,
                   16, 17, 18, 19, 20,
                   26, 27, 28, 29, 30
               };
    int q3[] = {  31, 32, 33, 34, 35,
                  41, 42, 43, 44, 45,
                  51, 52, 53, 54, 55
               };
    int q4[] = {  36, 37, 38, 39, 40,
                  46, 47, 48, 49, 50,
                  56, 57, 58, 59, 60
               };

    int i, n;
    int n1 = 0, n2 = 0, n3 = 0, n4 = 0;
    double media;

    for(i = 0; i < N; i++) {
        n = j->numeros[i];
        if(esta_em(n, q1, 15)) {
            n1++;
        } else if(esta_em(n, q2, 15)) {
            n2++;
        } else if(esta_em(n, q3, 15)) {
            n3++;
        } else if(esta_em(n, q4, 15)) {
            n4++;
        }
    }

    media = (n1 + n2 + n3 + n4) / 4.0;

    return (n1 == 0 || n2 == 0 || n3 == 0 || n4 == 0) && (abs(media - (N / 4)) <= 0.1);
}

int tem_equilibrio_par_impar(jogo *j)
{
    int i, n;
    int par = 0, impar = 0;

    for(i = 0; i < N; i++) {
        n = j->numeros[i];
        if(n % 2 == 0)
            par++;
        else
            impar++;
    }

    return abs(par - impar) <= 2;
}

int tem_n_ouros(jogo *j, int qtd)
{
    int dezenas_ouro[] = { 5, 7, 13, 14, 16, 25, 28, 29, 37, 38, 43, 44, 53, 54, 60 };

    int i, n;
    int ouros = 0;

    for(i = 0; i < N; i++) {
        n = j->numeros[i];
        if(esta_em(n, dezenas_ouro, 15)) {  /* temos 15 dezenas de ouro */
            ouros++;
        }
    }

    return ouros >= qtd;
}

/* ----------------------------------------------------------------- */

int cumpre_requisitos(jogo *j)
{
    return !termina_em_9_ou_0(j)
           && !eh_dupla(j)
           && !tem_seguidos(j)
           && !tem_mesma_coluna(j)
           && tem_equilibrio_quadrantes(j)
           && tem_equilibrio_par_impar(j)
           && tem_n_ouros(j, 3);
}

/* ----------------------------------------------------------------- */

void imprime_url(jogo *j)
{
    int i;

    for(i = 0; i < N; i++) {
        printf("%02d%s", j->numeros[i], i < N - 1 ? " " : "");
    }
    printf("\n");
}

/* ----------------------------------------------------------------- */

int main()
{
    int i, quantidade;
    jogo j;

    srand(time(NULL));

    printf("Quantos jogos? ");
    scanf("%d", &quantidade);

    for(i = 0; i < quantidade; i++) {
        do {
            preenche_jogo(&j);
        } while(!cumpre_requisitos(&j));
        /* imprime(j.numeros, N); */
        imprime_url(&j);
    }

    return EXIT_SUCCESS;
}
