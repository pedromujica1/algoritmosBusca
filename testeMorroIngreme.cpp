#include<stdio.h>
#include<math.h>
#include <time.h>
#include <stdlib.h>

// Problema das 8 Rainhas - Subida do Morro Mais Íngreme
#define N 8
int nosGerados = 0;
int nosExpandidos = 0;

// DOMÍNIO DO PROBLEMA: 

// Calcula a quantidade total de conflitos do tabuleiro
// Representação: o indice representa a coluna, e o valor armazenado as linhas 
// Como a representação garante uma rainha por coluna, os conflitos possíveis são apenas de linha ou diagonal
int calcularConflito(int vetor[]){ 
    
    int conflitos = 0;

    for(int i = 0; i<N; i++){
        for (int j = i + 1; j < N; j++){
            if(vetor[i] == vetor[j]){ // Verifica conflito na mesma linha
                conflitos++;
            } else if(abs(vetor[i] - vetor[j]) == abs(i - j)){ // Verifica conflito na mesma diagonal 
                conflitos++;
            }
        }
    }
    return conflitos;
}


void gerarEstadoAleatorio(int vetor[]){
    for(int i = 0; i < N; i++){
        vetor[i] = rand() % N;
    }
}


// Gera todos os vizinhos possíveis do estado atual e guarda aquele com o menor número de conflitos
// O vizinho é obtido movendo só uma rainha para outra linha da mesma coluna
void encontrarMelhorVizinho(int vetor[], int melhorVizinho[], int *melhorConflito){

    // Inicialmente o melhor estado é o atual
    for(int i=0; i<N; i++){
        melhorVizinho[i] = vetor[i];
    }
    
    
    for(int coluna = 0; coluna < N; coluna++){ // Testa mover cada rainha
        int linhaAtual = vetor[coluna];
        
        for(int novaLinha = 0; novaLinha < N; novaLinha++){ // Testa todas as linhas possíveis
            
            if (novaLinha == linhaAtual){ // ignora a posicao atual
                continue;
            }
            
            int vizinho[N];

            // Copia o estado atual
            for (int i = 0; i < N; i++) {
                vizinho[i] = vetor[i];
            }

            // Move uma rainha para gerar um vizinho
            vizinho[coluna] = novaLinha;

            nosGerados++;
            int conflitos = calcularConflito(vizinho);

            // Guarda o melhor vizinho encontrado
            if(conflitos < *melhorConflito){
                *melhorConflito = conflitos; 
                for (int i = 0; i < N; i++) {
                    melhorVizinho[i] = vizinho[i];
                }
            }
        }
    }
}

// SUBIDA DO MORRO MAIS ÍNGREME 
// Enquanto existir um vizinho melhor que o estado atual, o algoritmo se move para esse vizinho
// A busca termina quando nenhum vizinho apresenta melhora
void subidaMorroIngreme(int vetor[]){

    while(1){
        
        nosExpandidos++;

        int conflitoInicial = calcularConflito(vetor);
        int melhorConflito = conflitoInicial;

        printf("Conflitos atuais: %d\n", conflitoInicial);

        int melhorVizinho[N];

        encontrarMelhorVizinho(vetor, melhorVizinho, &melhorConflito);
        
        // Se não houve melhora, encerra a busca
        if (melhorConflito >= conflitoInicial){ 
            break;
        }

        // Move para o melhor vizinho encontrado
        for(int i = 0; i < N; i++) {
            vetor[i] = melhorVizinho[i];
        }

    }
}



int main(){

    srand(time(NULL));

    int vetor[N];
    int tentativas = 0;

    clock_t inicio = clock();

    // Reinício aleatório: se a busca parar em um ótimo local, gera um novo estado inicial e tenta dnv
    while(1){

        gerarEstadoAleatorio(vetor);

        tentativas++;

        subidaMorroIngreme(vetor);

        // Encerra quando encontrar uma solução sem conflitos
        if(calcularConflito(vetor) == 0){
            break;
        }
    }

    clock_t fim = clock();

    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    // Metricas ?????
    printf("\nCusto da solucao: %d conflitos\n", calcularConflito(vetor)); 
    printf("Numero de reinicios: %d tentativas\n", tentativas);  
    printf("Nos gerados: %d\n", nosGerados);
    printf("Nos expandidos: %d\n", nosExpandidos);
    printf("Tempo de execucao: %.6f segundos\n", tempo); 


    printf("\nPosicao final das rainhas:\n");
    for(int i = 0; i < N; i++){
        printf("%d ", vetor[i]);
    }


    return 0;
}