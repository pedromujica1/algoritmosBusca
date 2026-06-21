#include "hill_climbing.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
using namespace std;

//calcula a quantidade total de conflitos do tabuleiro
int calcularConflito(const vector<int>& vetor, int n) { 
    int conflitos = 0;
    for(int i = 0; i < n; i++){
        for (int j = i + 1; j < n; j++){
            if(vetor[i] == vetor[j]){ 
                conflitos++;
            } else if(abs(vetor[i] - vetor[j]) == abs(i - j)){ 
                conflitos++;
            }
        }
    }
    return conflitos;
}

//gera um estado inicial totalmente aleatório
vector<int> gerarEstadoAleatorio(int n) {
    vector<int> vetor(n);
    for(int i = 0; i < n; i++){
        vetor[i] = rand() % n;
    }
    return vetor;
}

//encontra o melhor vizinho modificando apenas uma rainha por vez
void encontrarMelhorVizinho(const vector<int>& vetor, int n, vector<int>& melhorVizinho, int& melhorConflito, long long& nosGerados) {
    melhorVizinho = vetor;
    
    for(int coluna = 0; coluna < n; coluna++){ 
        int linhaAtual = vetor[coluna];
        
        for(int novaLinha = 0; novaLinha < n; novaLinha++){ 
            if (novaLinha == linhaAtual){ 
                continue;
            }
            
            vector<int> vizinho = vetor;
            vizinho[coluna] = novaLinha;
            nosGerados++;

            int conflitos = calcularConflito(vizinho, n);

            if(conflitos < melhorConflito){
                melhorConflito = conflitos; 
                melhorVizinho = vizinho;
            }
        }
    }
}

//executa uma única subida de morro até atingir um ótimo local
void subidaMorroIngreme(vector<int>& vetor, int n, long long& nosGerados, long long& nosExpandidos) {
    while(true){
        nosExpandidos++;

        int conflitoInicial = calcularConflito(vetor, n);
        int melhorConflito = conflitoInicial;

        vector<int> melhorVizinho;
        encontrarMelhorVizinho(vetor, n, melhorVizinho, melhorConflito, nosGerados);
        
        if (melhorConflito >= conflitoInicial){ 
            break; // Travou em um ótimo local ou solução global
        }

        vetor = melhorVizinho;
    }
}

//gerenciador do Hill Climbing que aplica os reinícios caso trave
MetricasHillClimbing executaHill_climbing(int n_rainhas) {
    auto inicio = chrono::high_resolution_clock::now();

    long long nosGerados = 0;
    long long nosExpandidos = 0;
    int tentativas = 0;
    
    vector<int> vetor;

    while(true){
        vetor = gerarEstadoAleatorio(n_rainhas);
        tentativas++;

        subidaMorroIngreme(vetor, n_rainhas, nosGerados, nosExpandidos);

        // Se encontrou a solução sem conflitos, encerra
        if(calcularConflito(vetor, n_rainhas) == 0){
            break;
        }
    }

    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);

    // Mapeia os dados para a struct estruturada de retorno
    MetricasHillClimbing resultado;
    resultado.solucao = vetor;
    resultado.nosGerados = nosGerados;
    resultado.nosExpandidos = nosExpandidos;
    resultado.reinicios = tentativas;
    resultado.tempoExecucaoMs = duracao.count();

    return resultado;
}