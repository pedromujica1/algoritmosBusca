#include "hill_climbing.h"
#include "astar.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <random>
using namespace std;


vector<int> gerarEstadoAleatorio(int n, mt19937& rng) {
    vector<int> vetor(n);

    uniform_int_distribution<int> dist(0, n - 1);

    for(int i = 0; i < n; i++) {
        vetor[i] = dist(rng);
    }

    return vetor;
}


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
MetricasHillClimbing executaHill_climbing(int n_rainhas, const vector<int> &estadoInicial) {

    auto inicio = chrono::high_resolution_clock::now();
    
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

    MetricasHillClimbing resultado;

    resultado.solucaoInicial = estadoInicial;
    resultado.solucao = vetor;
    resultado.nosGerados = nosGerados;
    resultado.nosExpandidos = nosExpandidos;
    resultado.reinicios = tentativas-1;
    resultado.tempoExecucaoMs = duracao.count();
    resultado.sementeUsada = sementeGerada;
    return resultado;
  
}

vector<MetricasHillClimbing> benchmarkHill_climbing() {
    
    vector<MetricasHillClimbing> resultados;
    int n_rainhas = 8;

    random_device rd;
    mt19937 rng(rd());

    for(int i = 0; i < 5; i++) {       
        //gera estadios iniciais
        vector<int> estadoInicialParaTeste = gerarEstadoAleatorio(n_rainhas, rng);

        //passa resultado pra execução
        MetricasHillClimbing resultado = executaHill_climbing(n_rainhas, estadoInicialParaTeste);
        resultados.push_back(resultado);
    }

    return resultados;
}