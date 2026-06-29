#include "estruturas.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <random>
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

//encontra o melhor vizinho modificando apenas uma rainha por vez
void encontrarMelhorVizinho(const vector<int>& vetor, int n, vector<int>& melhorVizinho, int& melhorConflito, long long &nosGerados) {
    melhorVizinho = vetor;
    
    for(int linha = 0; linha < n; linha++){ 
        int colunaAtual = vetor[linha];
        
        for(int novaColuna = 0; novaColuna < n; novaColuna++){ 
            if (novaColuna == colunaAtual){ 
                continue;
            }
            
            vector<int> vizinho = vetor;
            vizinho[linha] = novaColuna;
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
MetricasBusca executaHill_climbing(int n_rainhas, const vector<int> &estadoInicial) {

    auto inicio = chrono::high_resolution_clock::now();

    random_device rd;
    //semente com Hardware para garantir alta randomicidade
    unsigned int sementeGerada = rd();
    //inicializa o gerador usando essa semente capturada
    mt19937 rng(sementeGerada);

    long long nosGerados = 0;
    long long nosExpandidos = 0;
    int tentativas = 0;

    //vetor do usuario
    vector<int> vetor = estadoInicial;

    while(true){

        //na primeira tentativa, usamos o vetor do usuário.
        //se trava é gerado um novamente
        if (tentativas > 0) {
            vetor = gerarEstadoAleatorio(n_rainhas, rng);
        }
        tentativas++;

        subidaMorroIngreme(vetor,n_rainhas,nosGerados,nosExpandidos);

        if(calcularConflito(vetor, n_rainhas) == 0){
            break;
        }
    }

    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);

    MetricasBusca resultado;

    resultado.solucaoInicial = estadoInicial;
    resultado.solucao = vetor;
    resultado.nosGerados = nosGerados;
    resultado.nosExpandidos = nosExpandidos;
    resultado.reiniciosHill = tentativas-1;
    resultado.tempoExecucaoMs = duracao.count();
    resultado.sementeUsada = sementeGerada;
    return resultado;
  
}

vector<MetricasBusca> benchmarkHill_climbing() {
    
    vector<MetricasBusca> resultados;
    int n_rainhas = 8;

    random_device rd;
    mt19937 rng(rd());

    for(int i = 0; i < 5; i++) {       
        //gera estadios iniciais
        vector<int> estadoInicialParaTeste = gerarEstadoAleatorio(n_rainhas, rng);

        //passa resultado pra execução
        MetricasBusca resultado = executaHill_climbing(n_rainhas, estadoInicialParaTeste);
        resultados.push_back(resultado);
    }

    return resultados;
}
