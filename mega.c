#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N     6
#define SENA  60

int compar(const void *a, const void *b)
{
    int *na = (int *)a;
    int *nb = (int *)b;

    return (*na - *nb);
}

int esta_em(int e, int vetor[], int n)
{
    int i, achou = 0;

    for(i = 0; i < n; i++) {
        if(e == vetor[i]) {
            achou = 1;
            break;
        }
    }

    return achou;
}

int main()
{
    int sorteio, jogos, jogo;
    int i, min_j, max_j, j, n, numeros[N];

    srand(time(NULL));

    printf("Número do sorteio? ");
    scanf("%d", &sorteio);

    printf("Quantos jogos? ");
    scanf("%d", &jogos);

    for(jogo = 1; jogo <= jogos; jogo++) {
        for(i = 0; i < N; i++) {
            min_j = rand() % sorteio;
            max_j = min_j + rand() % sorteio;

            for(j = min_j; j < max_j; j++) {
                n = rand() % SENA + 1;
            }

            if(esta_em(n, numeros, i)) {
                i--;
                continue;
            }

            numeros[i] = n;
        }

        qsort(numeros, N, sizeof(int), compar);
        printf("Jogo %d: ", jogo);
        for(i = 0; i < N; i++) {
            printf("%02d%s", numeros[i], i < N - 1 ? " - " : "");
        }
        printf("\n");
    }

    return EXIT_SUCCESS;
}
